#include "taskdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QScrollArea>

// ── GitHub Dark palette ──────────────────────────────────────────────────
#define GH_BG          "#0d1117"
#define GH_CARD        "#161b22"
#define GH_BORDER      "#30363d"
#define GH_TEXT        "#e6edf3"
#define GH_MUTED       "#8b949e"
#define GH_BLUE        "#388bfd"
#define GH_BLUE_H      "#58a6ff"
#define GH_INPUT_BG    "#0d1117"
#define FONT_FAMILY    "Segoe UI"

TaskDialog::TaskDialog(QWidget *parent)
    : QDialog(parent)
{
    setStyleSheet(QString(
        "QDialog { background-color: %1; color: %2; font-family: '%3'; font-size: 10pt; }"
        "QLabel  { background: transparent; color: %2; }"
        "QScrollBar:vertical { background: %1; width: 8px; border: none; }"
        "QScrollBar::handle:vertical { background: %4; border-radius: 4px; }"
    ).arg(GH_BG).arg(GH_TEXT).arg(FONT_FAMILY).arg(GH_BORDER));
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

    // ── Title ──────────────────────────────────────────────────────────
    titleLabel = new QLabel(this);
    titleLabel->setText(QString("<b style='font-size:16pt; color:%1;'>Задание</b>").arg(GH_TEXT));
    titleLabel->setTextFormat(Qt::RichText);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // ── Separator ─────────────────────────────────────────────────────
    auto makeSep = [&]() {
        QFrame *sep = new QFrame(this);
        sep->setFrameShape(QFrame::HLine);
        sep->setStyleSheet(QString("QFrame { background: %1; border: none; max-height: 1px; }").arg(GH_BORDER));
        return sep;
    };
    mainLayout->addWidget(makeSep());

    // ── Work title ──────────────────────────────────────────────────
    workTitleLabel = new QLabel(this);
    workTitleLabel->setText(QString(
        "<p style='font-size:12pt; font-weight:bold; color:%1; text-align:center;'>"
        "Графическое отображение ветвящейся функции<br>"
        "в рамках клиент-серверного проекта"
        "</p>"
    ).arg(GH_TEXT));
    workTitleLabel->setTextFormat(Qt::RichText);
    workTitleLabel->setWordWrap(true);
    workTitleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(workTitleLabel);

    // ── Group ────────────────────────────────────────────────────────────
    groupLabel = new QLabel(this);
    groupLabel->setText(QString(
        "<p style='font-size:11pt; color:%1; text-align:center;'><b>Подгруппа 5</b></p>"
    ).arg(GH_MUTED));
    groupLabel->setTextFormat(Qt::RichText);
    groupLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(groupLabel);

    // ── Members card ──────────────────────────────────────────────────
    membersLabel = new QLabel(this);
    membersLabel->setText(QString(
        "<p style='font-size:10pt; color:%1; line-height:1.8;'>"
        "<b style='color:%2;'>Участники:</b><br>"
        "&nbsp;&nbsp;&bull;&nbsp;Орлов Руслан<br>"
        "&nbsp;&nbsp;&bull;&nbsp;Карелин Кирилл<br>"
        "&nbsp;&nbsp;&bull;&nbsp;Серёгина Елизавета<br>"
        "&nbsp;&nbsp;&bull;&nbsp;Воробьёва Елизавета<br>"
        "&nbsp;&nbsp;&bull;&nbsp;Сарафанов Алексей"
        "</p>"
    ).arg(GH_TEXT).arg(GH_TEXT));
    membersLabel->setTextFormat(Qt::RichText);
    membersLabel->setStyleSheet(QString(
        "QLabel {"
        "  background-color: %1;"
        "  border: 1px solid %2;"
        "  border-radius: 6px;"
        "  padding: 10px;"
        "}"
    ).arg(GH_CARD).arg(GH_BORDER));
    mainLayout->addWidget(membersLabel);

    mainLayout->addWidget(makeSep());

    // ── Formula card ───────────────────────────────────────────────────
    formulaLabel = new QLabel(this);
    formulaLabel->setText(
        "<p style='font-size:10pt; margin:0; line-height:1.8;'>"
        "<b style='color:#e6edf3;'>Функция &numero;9:</b><br>"
        "<span style='color:#f85149;'>|x &middot; a| &minus; 2,&nbsp;&nbsp; x &lt; &minus;2</span><br>"
        "<span style='color:#3fb950;'>b &middot; (x&sup2;) + x + 1,&nbsp;&nbsp; &minus;2 &le; x &lt; 2</span><br>"
        "<span style='color:#58a6ff;'>|x &minus; 2| + 1 &middot; c,&nbsp;&nbsp; x &ge; 2</span>"
        "</p>"
    );
    formulaLabel->setTextFormat(Qt::RichText);
    formulaLabel->setStyleSheet(QString(
        "QLabel {"
        "  background-color: %1;"
        "  border: 1px solid %2;"
        "  border-radius: 6px;"
        "  padding: 10px;"
        "}"
    ).arg(GH_INPUT_BG).arg(GH_BORDER));
    formulaLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    mainLayout->addWidget(formulaLabel);

    mainLayout->addStretch(1);

    // ── Close button ──────────────────────────────────────────────────
    closeBtn = new QPushButton("Закрыть", this);
    closeBtn->setMinimumHeight(36);
    closeBtn->setStyleSheet(QString(
        "QPushButton {"
        "  background-color: %1; color: #ffffff;"
        "  border: 1px solid rgba(240,246,252,0.1); border-radius: 6px;"
        "  font-size: 11pt; padding: 4px 20px; font-family: '%3';"
        "}"
        "QPushButton:hover { background-color: %2; }"
    ).arg(GH_BLUE).arg(GH_BLUE_H).arg(FONT_FAMILY));
    QHBoxLayout *btnRow = new QHBoxLayout();
    btnRow->addStretch(1);
    btnRow->addWidget(closeBtn);
    btnRow->addStretch(1);
    mainLayout->addLayout(btnRow);

    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    setLayout(mainLayout);
}
