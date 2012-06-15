//                              -*- Mode: C++ -*-
//
// Filename: controller.hxx
// Author: Frederic DELBOS - fred.delbos@gmail.com
// Created: Mon Jul  4 12:10:38 2011 (+0200)
// Last-Updated: Mon Jul  4 19:00:16 2011 (+0200)
//           By: Frederic DELBOS - fred.delbos@gmail.com
//     Update #: 18
//
//

#pragma once

#include <QDialog>
#include <QTcpSocket>
#include <QTime>
#include "ui_zappytester.h"


class Controller : public QDialog
{
  Q_OBJECT

public:
  Controller(QWidget *parent = 0);
  void sendMsg(QString);

private:
  Ui_Dialog *_uidialog;
  QTcpSocket *_socket;
  QTime *_clock;

  void addReceivedText(QString);
  void addSendText(QString);
  void disableCommands();
  void enableCommands();
  QString getTimeElapsed();

private slots:
  /* connection */
  void newConnection();
  void sockError(QAbstractSocket::SocketError);
  void sockRead();
  void sockConnected();
  void connectionClosedByServer();

  void sendAvance();
  void sendGauche();
  void sendDroite();
  void sendVoir();
  void sendInventaire();
  void sendFork();
  void sendPrend();
  void sendPose();
  void sendExpulse();
  void sendIncantation();
  void sendConnectNbr();
  void sendBroadcast();
  void sendBidon();
};
