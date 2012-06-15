//                              -*- Mode: C++ -*- 
// 
// Filename: main.cxx
// Author: Frederic DELBOS - fred.delbos@gmail.com
// Created: Mon Jul  4 12:03:17 2011 (+0200)
// Last-Updated: Mon Jul  4 18:55:27 2011 (+0200)
//           By: Frederic DELBOS - fred.delbos@gmail.com
//     Update #: 5
// 
// 

#include <QApplication>
#include "controller.hxx"

int main(int ac, char **av)
{
  QApplication app(ac, av);
  Controller controller;
  controller.show();
  return app.exec();
}
