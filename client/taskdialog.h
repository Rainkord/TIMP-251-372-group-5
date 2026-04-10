#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class TaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TaskDialog(QWidget *parent = nullptr);
    ~TaskDialog();

private:
    QLabel      *titleLabel;
    QLabel      *workTitleLabel;
    QLabel      *groupLabel;
    QLabel      *membersLabel;
    QLabel      *formulaLabel;
    QPushButton *closeBtn;

    void setupUI();
};

#endif // TASKDIALOG_H
