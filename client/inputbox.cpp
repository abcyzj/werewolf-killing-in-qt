#include "inputbox.h"

#include <QEvent>

InputBox::InputBox(const QString &text, QWidget *parent): QLineEdit(text, parent){
  installEventFilter(this);
  setFont(QFont("黑体", 20));
}

InputBox::~InputBox(){

}

void InputBox::enableInput(){
  setMaxLength(200);
}

void InputBox::disableInput(){
  clear();
  setMaxLength(0);
}

bool InputBox::eventFilter(QObject *watched, QEvent *event){
  if(watched == this && event->type() == QEvent::FocusOut)
    return true;
  else
    return QLineEdit::eventFilter(watched, event);
}
