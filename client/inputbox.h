#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <QLineEdit>

class InputBox : public QLineEdit{
  Q_OBJECT

public:
  InputBox(const QString &text, QWidget *parent = 0);
  ~InputBox();

public slots:
  void enableInput();
  void disableInput();

protected:
  virtual bool eventFilter(QObject *watched, QEvent *event);
};

#endif // INPUTBOX_H
