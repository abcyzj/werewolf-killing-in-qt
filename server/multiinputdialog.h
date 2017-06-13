/**************************************
名称：multiinputdialog.h
作者：叶梓杰 计65 2016011380
时间：2017/05/22
内容：有多个输入框的对话框
版权：借鉴了某个CSDN博客
**************************************/

#ifndef MULTIINPUTDIALOG_H
#define MULTIINPUTDIALOG_H


#include <QDialog>

class QLabel;
class QLineEdit;

class MultiInputDialog : public QDialog
{
	Q_OBJECT
 private:
	const int m_GroupCount;
	QLabel **m_Labels;
	QLineEdit **m_LineEdits;
	QPushButton *m_OKButton;
	QPushButton *m_CancelButton;

 public:
	MultiInputDialog(int count, QWidget *parent = 0);
	virtual ~MultiInputDialog();
	void SetLabelTexts(const QStringList &listText);
	void SetOneLabelText(int index, const QString &text);
	QString GetOneText(int index);
	QStringList GetAllTexts();
	//为了让多组输入框看起来整齐些，所以我固定了QLabel的宽度
	void SetLabelsWidth(int width);
	//使用正则表达式来限制输入的字符
	void SetLineEditRegExp(int index, QRegExp regExp);

	//请根据需要重写这两个函数
	virtual void accept() { QDialog::accept(); }
	virtual void reject() { QDialog::reject(); }
};

#endif // MULTIINPUTDIALOG_H
