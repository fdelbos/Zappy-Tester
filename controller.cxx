//                              -*- Mode: C++ -*- 
// 
// Filename: controller.cxx
// Author: Frederic DELBOS - fred.delbos@gmail.com
// Created: Mon Jul  4 12:15:47 2011 (+0200)
// Last-Updated: Mon Jul  4 19:38:07 2011 (+0200)
//           By: Frederic DELBOS - fred.delbos@gmail.com
//     Update #: 63
// 
// 

#include "controller.hxx"
#include <QTextStream>
#include <iostream>
#include <QMessageBox>


Controller::Controller(QWidget *parent)
{
  (void)parent;
  _uidialog = new Ui_Dialog;
  _uidialog->setupUi(this);
  disableCommands();
  _clock = 0;

  _socket = new QTcpSocket(this);
  connect(_uidialog->connection, SIGNAL(pressed()), this, SLOT(newConnection()));
  connect(_socket, SIGNAL(connected()), this, SLOT(sockConnected()));
  connect(_socket, SIGNAL(disconnected()), this, SLOT(connectionClosedByServer()));
  connect(_socket, SIGNAL(readyRead()), this, SLOT(sockRead()));
  connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)),
	  this, SLOT(sockError(QAbstractSocket::SocketError)));

  connect(_uidialog->avance, SIGNAL(pressed()), this, SLOT(sendAvance()));
  connect(_uidialog->gauche, SIGNAL(pressed()), this, SLOT(sendGauche()));
  connect(_uidialog->droite, SIGNAL(pressed()), this, SLOT(sendDroite()));
  connect(_uidialog->voir, SIGNAL(pressed()), this, SLOT(sendVoir()));
  connect(_uidialog->inventaire, SIGNAL(pressed()), this, SLOT(sendInventaire()));
  connect(_uidialog->fork, SIGNAL(pressed()), this, SLOT(sendFork()));
  connect(_uidialog->incantation, SIGNAL(pressed()), this, SLOT(sendIncantation()));
  connect(_uidialog->expulse, SIGNAL(pressed()), this, SLOT(sendExpulse()));
  connect(_uidialog->prend, SIGNAL(pressed()), this, SLOT(sendPrend()));
  connect(_uidialog->pose, SIGNAL(pressed()), this, SLOT(sendPose()));
  connect(_uidialog->connectNbr, SIGNAL(pressed()), this, SLOT(sendConnectNbr()));
  connect(_uidialog->broadcast, SIGNAL(pressed()), this, SLOT(sendBroadcast()));
  connect(_uidialog->commandeBidon, SIGNAL(pressed()), this, SLOT(sendBidon()));
}

void Controller::newConnection()
{
    if(_uidialog->connection->text() == tr("Connexion"))
    {
      _socket->abort();
      _socket->connectToHost(_uidialog->host->text(), _uidialog->port->text().toInt());
    }
  else
    {
      _socket->abort();
      _uidialog->connection->setText(tr("Connexion"));
      disableCommands();
    }
}

void Controller::sockRead()
{
  QString msg = "";
  char buf[2048];
  qint64 lineLength = 0;
  while((lineLength = _socket->readLine(buf, sizeof(buf))) > 0)
      msg += tr(buf);
  addReceivedText(msg);
  if(!msg.compare("BIENVENUE\n"))
    sendMsg(_uidialog->team->text()); // send team name
}

QString Controller::getTimeElapsed()
{
  if(!_clock)
    return tr("");
  int time = _clock->elapsed();
  int sec = time / 1000;
  int milli = time % 1000;
  QString str = QString("%1").arg(milli);
  while(milli < 100)
    {
      str = tr("0") + str;
      milli *= 10;
    }
  str = QString("%1").arg(sec) + tr(",") + str;
  str = tr("<i style='color:green'>") + str + tr(" -></i> ");
  return str;
}

void Controller::addReceivedText(QString text)
{
  text = tr("<hr /><div style='color:red'>") + getTimeElapsed() + text + tr("</div>");
  _uidialog->shell->setHtml(_uidialog->shell->toHtml() + text);
  QTextCursor c = _uidialog->shell->textCursor();
  c.movePosition(QTextCursor::End);
  _uidialog->shell->setTextCursor(c);
}

void Controller::addSendText(QString text)
{
  text = tr("<hr /><div style='color:blue'>") + getTimeElapsed() + text + tr("</div>");
  _uidialog->shell->setHtml(_uidialog->shell->toHtml() + text);
  QTextCursor c = _uidialog->shell->textCursor();
  c.movePosition(QTextCursor::End);
  _uidialog->shell->setTextCursor(c);
}

void Controller::sockError(QAbstractSocket::SocketError error)
{
  disableCommands();
  QMessageBox msgBox;
  switch(error)
    {
    case QAbstractSocket::ConnectionRefusedError:
      msgBox.setText("Connection refused"); break;
    case QAbstractSocket::RemoteHostClosedError:
      msgBox.setText("Remote Host closed connection"); break;
    case QAbstractSocket::HostNotFoundError:
      msgBox.setText("Host not found"); break;
    case QAbstractSocket::SocketAccessError:
      msgBox.setText("Socket access error"); break;
    case QAbstractSocket::SocketTimeoutError:
      msgBox.setText("Connection timeout"); break;
    case QAbstractSocket::DatagramTooLargeError:
      msgBox.setText("Datagram too large error"); break;
    default:
      msgBox.setText("Unknow network error"); break;
    }
  msgBox.exec();
}

void Controller::sockConnected()
{
  enableCommands();
}

void Controller::connectionClosedByServer()
{
  disableCommands();
  QMessageBox msgBox;
  msgBox.setText("Connection closed by server");
}

void Controller::sendMsg(QString msg)
{
  addSendText(msg);
  msg += '\n';
  QByteArray ba = msg.toLocal8Bit();
  const char *c_str = ba.data();
  _socket->write(c_str, msg.size());
}

void Controller::enableCommands()
{
  if(_clock)
    delete _clock;
  _clock = new QTime;
  _clock->start();
  _uidialog->host->setEnabled(false);
  _uidialog->port->setEnabled(false);
  _uidialog->team->setEnabled(false);
  _uidialog->shell->setHtml("");
  _uidialog->connection->setText(tr("Deconnexion"));
  _uidialog->commandsBox->setEnabled(true);
}

void Controller::disableCommands()
{
  _uidialog->host->setEnabled(true);
  _uidialog->port->setEnabled(true);
  _uidialog->team->setEnabled(true);
  _uidialog->connection->setText(tr("Connexion"));
  _uidialog->commandsBox->setEnabled(false);
}

void Controller::sendAvance()
{
  sendMsg(tr("avance"));
}

void Controller::sendGauche()
{
  sendMsg(tr("gauche"));
}

void Controller::sendDroite()
{
  sendMsg(tr("droite"));
}

void Controller::sendVoir()
{
  sendMsg(tr("voir"));
}

void Controller::sendInventaire()
{
  sendMsg(tr("inventaire"));
}

void Controller::sendFork()
{
  sendMsg(tr("fork"));
}

void Controller::sendPrend()
{
  sendMsg(tr("prend ") + _uidialog->prendSelect->currentText());
}

void Controller::sendPose()
{
  sendMsg(tr("pose ") + _uidialog->poseSelect->currentText());
}

void Controller::sendExpulse()
{
  sendMsg(tr("expulse"));
}

void Controller::sendIncantation()
{
  sendMsg(tr("incantation"));
}

void Controller::sendConnectNbr()
{
  sendMsg(tr("connect_nbr"));
}

void Controller::sendBroadcast()
{
  sendMsg(tr("broadcast ") + _uidialog->broadcastMsg->text());
}

void Controller::sendBidon()
{
  sendMsg(_uidialog->commandeBidonMsg->text());
}
