#include "taskdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QScrollArea>

TaskDialog::TaskDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
}

TaskDialog::~TaskDialog() {}

void TaskDialog::setupUI()
{
    setWindowTitle("Задание — Подгруппа 5");
    resize(500, 560);
    setMinimumSize(400, 450);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(12);

    // ── Title ────────────────────────────────────
    titleLabel = new QLabel(this);
    titleLabel->setText("<b style='font-size:16pt;'>Задание</b>");
    titleLabel->setTextFormat(Qt::RichText);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // ── Separator ────────────────────────────────
    QFrame *sep1 = new QFrame(this);
    sep1->setFrameShape(QFrame::HLine);
    sep1->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(sep1);

    // ── Work title ───────────────────────────────
    workTitleLabel = new QLabel(this);
    workTitleLabel->setText(
        "<p style='font-size:13pt; font-weight:bold; color:#2c3e50; text-align:center;'>"
        "Графическое отображение ветвящейся функции<br>"
        "в рамках клиент-серверного проекта"
        "</p>"
    );
    workTitleLabel->setTextFormat(Qt::RichText);
    workTitleLabel->setWordWrap(true);
    workTitleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(workTitleLabel);

    // ── Group ─────────────────────────────────────
    groupLabel = new QLabel(this);
    groupLabel->setText(
        "<p style='font-size:12pt; color:#34495e;'>"
        "<b>Подгруппа 5</b>"
        "</p>"
    );
    groupLabel->setTextFormat(Qt::RichText);
    groupLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(groupLabel);

    // ── Members ───────────────────────────────────
    membersLabel = new QLabel(this);
    membersLabel->setText(
        "<p style='font-size:11pt;'>"
        "<b>Участники:</b><br>"
        "&nbsp;&nbsp;&bull;&nbsp;Орлов Руслан<br>"
        "&nbsp;&nbsp;&bull;&nbsp;Карелин Кирилл<br>"
        "&nbsp;&nbsp;&bull;&nbsp;Серёгина Елизавета<br>"
        "&nbsp;&nbsp;&bull;&nbsp;Воробьёва Елизавета<br>"
        "&nbsp;&nbsp;&bull;&nbsp;Сарафанов Алексей"
        "</p>"
    );
    membersLabel->setTextFormat(Qt::RichText);
    membersLabel->setStyleSheet(
        "QLabel { background: #ecf0f1; border: 1px solid #bdc3c7; "
        "border-radius: 6px; padding: 10px; }"
    );
    mainLayout->addWidget(membersLabel);

    // ── Separator ────────────────────────────────
    QFrame *sep2 = new QFrame(this);
    sep2->setFrameShape(QFrame::HLine);
    sep2->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(sep2);

    // ── Formula ───────────────────────────────────
    formulaLabel = new QLabel(this);
    formulaLabel->setText(
        "<p style='font-size:10pt; margin:0; line-height:1.8;'>"
        "<b>Функция #9:</b><br>"
        "<span style='color:red;'>|x &middot; a| &minus; 2,&nbsp;&nbsp; x &lt; &minus;2</span><br>"
        "<span style='color:green;'>b &middot; (x&sup2;) + x + 1,&nbsp;&nbsp; &minus;2 &le; x &lt; 2</span><br>"
        "<span style='color:blue;'>|x &minus; 2| + 1 &middot; c,&nbsp;&nbsp; x &ge; 2</span>"
        "</p>"
    );
    formulaLabel->setTextFormat(Qt::RichText);
    formulaLabel->setStyleSheet(
        "QLabel { background: white; border: 1px solid #ccc; "
        "border-radius: 6px; padding: 10px; }"
    );
    formulaLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    mainLayout->addWidget(formulaLabel);

    mainLayout->addStretch(1);

    // ── Close button ──────────────────────────────
    closeBtn = new QPushButton("Закрыть", this);
    closeBtn->setMinimumHeight(36);
    closeBtn->setStyleSheet(
        "QPushButton { background-color: #3498db; color: white; "
        "border-radius: 5px; font-size: 12pt; padding: 4px 16px; }"
        "QPushButton:hover { background-color: #2980b9; }"
    );
    QHBoxLayout *btnRow = new QHBoxLayout();
    btnRow->addStretch(1);
    btnRow->addWidget(closeBtn);
    btnRow->addStretch(1);
    mainLayout->addLayout(btnRow);

    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);

    setLayout(mainLayout);
}
