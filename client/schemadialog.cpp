#include "schemadialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QScrollArea>
#include <QPen>
#include <QFont>
#include <QPolygon>

// ── GitHub Dark colours ──────────────────────────────────────────────────
// Canvas
#define FC_BG          QColor(0x0d, 0x11, 0x17)   // #0d1117
#define FC_BORDER      QColor(0x30, 0x36, 0x3d)   // #30363d
#define FC_TEXT        QColor(0xe6, 0xed, 0xf3)   // #e6edf3
#define FC_ARROW       QColor(0x8b, 0x94, 0x9e)   // #8b949e
#define FC_LABEL       QColor(0x8b, 0x94, 0x9e)   // #8b949e
// Block fills
#define FC_GREEN_FILL  QColor(0x1a, 0x35, 0x1e)   // start/end
#define FC_BLUE_FILL   QColor(0x1a, 0x27, 0x40)   // input
#define FC_YELLOW_FILL QColor(0x2d, 0x28, 0x12)   // diamond
#define FC_RED_FILL    QColor(0x3a, 0x18, 0x18)   // branch 1
#define FC_TEAL_FILL   QColor(0x12, 0x2d, 0x2a)   // branch 2
#define FC_PURPLE_FILL QColor(0x1f, 0x1a, 0x3a)   // branch 3
#define FC_OUT_FILL    QColor(0x1a, 0x28, 0x2d)   // output

#define FONT_FAMILY    "Segoe UI"

// ───────────────────────────────────────────────────────────────────
FlowchartWidget::FlowchartWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(700, 1000);
}

void FlowchartWidget::drawRoundedBlock(QPainter &p, int cx, int cy, int w, int h,
                                        const QString &text, const QColor &fill)
{
    QRect rect(cx - w/2, cy - h/2, w, h);
    QPainterPath path;
    path.addRoundedRect(rect, 10, 10);
    p.fillPath(path, fill);
    p.setPen(QPen(FC_BORDER, 1.5));
    p.drawPath(path);
    p.setPen(FC_TEXT);
    QFont f(FONT_FAMILY, 10);
    p.setFont(f);
    p.drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, text);
}

void FlowchartWidget::drawDiamond(QPainter &p, int cx, int cy, int w, int h,
                                   const QString &text, const QColor &fill)
{
    QPolygon diamond;
    diamond << QPoint(cx, cy - h/2)
            << QPoint(cx + w/2, cy)
            << QPoint(cx, cy + h/2)
            << QPoint(cx - w/2, cy);
    QPainterPath path;
    path.addPolygon(diamond);
    path.closeSubpath();
    p.fillPath(path, fill);
    p.setPen(QPen(FC_BORDER, 1.5));
    p.drawPath(path);
    p.setPen(FC_TEXT);
    QFont f(FONT_FAMILY, 9);
    p.setFont(f);
    QRect textRect(cx - w/2 + 15, cy - h/2 + 10, w - 30, h - 20);
    p.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, text);
}

void FlowchartWidget::drawArrowDown(QPainter &p, int cx, int y1, int y2)
{
    p.setPen(QPen(FC_ARROW, 2));
    p.drawLine(cx, y1, cx, y2);
    QPolygon arrow;
    arrow << QPoint(cx, y2) << QPoint(cx-6, y2-10) << QPoint(cx+6, y2-10);
    p.setBrush(FC_ARROW);
    p.drawPolygon(arrow);
    p.setBrush(Qt::NoBrush);
}

void FlowchartWidget::drawArrowRight(QPainter &p, int x1, int x2, int y)
{
    p.setPen(QPen(FC_ARROW, 2));
    p.drawLine(x1, y, x2, y);
    QPolygon arrow;
    arrow << QPoint(x2, y) << QPoint(x2-10, y-6) << QPoint(x2-10, y+6);
    p.setBrush(FC_ARROW);
    p.drawPolygon(arrow);
    p.setBrush(Qt::NoBrush);
}

void FlowchartWidget::drawArrowLine(QPainter &p, int x1, int y1, int x2, int y2)
{
    p.setPen(QPen(FC_ARROW, 2));
    p.drawLine(x1, y1, x2, y2);
}

void FlowchartWidget::drawText(QPainter &p, int cx, int cy, int w, int h, const QString &text)
{
    QRect rect(cx - w/2, cy - h/2, w, h);
    QFont f(FONT_FAMILY, 9);
    f.setBold(true);
    p.setFont(f);
    p.setPen(FC_LABEL);
    p.drawText(rect, Qt::AlignCenter, text);
    f.setBold(false);
    p.setFont(f);
}

void FlowchartWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Тёмный фон
    p.fillRect(rect(), FC_BG);

    int centerX = 220;
    int rightX  = 520;
    int bw = 200, bh = 45;
    int dw = 240, dh = 80;
    int gap = 30;
    int routeX = rightX + bw/2 + 25;
    int y = 40;

    // 1. Начало
    drawRoundedBlock(p, centerX, y, 140, 40, "Начало", FC_GREEN_FILL);
    int y1 = y + 20;
    y += 40 + gap;
    drawArrowDown(p, centerX, y1, y - bh/2);

    // 2. Ввод
    drawRoundedBlock(p, centerX, y, bw, bh, "Ввод x, a, b, c", FC_BLUE_FILL);
    y1 = y + bh/2;
    y += bh + gap;
    drawArrowDown(p, centerX, y1, y - dh/2);

    // 3. x < -2 ?
    int diamondY1 = y;
    drawDiamond(p, centerX, y, dw, dh, "x < -2 ?", FC_YELLOW_FILL);
    drawArrowRight(p, centerX + dw/2, rightX - bw/2, y);
    drawText(p, centerX + dw/2 + 25, y - 14, 40, 20, "Да");
    int rb1CenterY = diamondY1;
    drawRoundedBlock(p, rightX, rb1CenterY, bw, bh, "f = |x\u00B7a| \u2212 2", FC_RED_FILL);
    int rb1Bottom = rb1CenterY + bh/2;

    y1 = y + dh/2;
    y += dh + gap;
    drawArrowDown(p, centerX, y1, y - dh/2);
    drawText(p, centerX + 16, y1 + 10, 40, 20, "Нет");

    // 4. -2 ≤ x < 2 ?
    int diamondY2 = y;
    drawDiamond(p, centerX, y, dw, dh, "-2 \u2264 x < 2 ?", FC_YELLOW_FILL);
    drawArrowRight(p, centerX + dw/2, rightX - bw/2, y);
    drawText(p, centerX + dw/2 + 25, y - 14, 40, 20, "Да");
    int rb2CenterY = diamondY2;
    drawRoundedBlock(p, rightX, rb2CenterY, bw, bh, "f = b\u00B7(x\u00B2) + x + 1", FC_TEAL_FILL);
    int rb2Bottom = rb2CenterY + bh/2;

    y1 = y + dh/2;
    y += dh + gap;
    drawArrowDown(p, centerX, y1, y - bh/2);
    drawText(p, centerX + 16, y1 + 10, 40, 20, "Нет");

    // 5. Ветвь 3
    int block3CenterY = y;
    drawRoundedBlock(p, centerX, block3CenterY, bw, bh, "f = |x \u2212 2| + 1\u00B7c", FC_PURPLE_FILL);
    int block3Bottom = block3CenterY + bh/2;

    // Точка слияния
    int mergeY = block3Bottom + gap + 20;

    drawArrowLine(p, centerX, block3Bottom, centerX, mergeY);

    p.setPen(QPen(FC_ARROW, 2));
    p.drawLine(rightX + bw/2, rb1Bottom, routeX, rb1Bottom);
    p.drawLine(routeX, rb1Bottom, routeX, mergeY);
    p.drawLine(rightX + bw/2, rb2Bottom, routeX, rb2Bottom);
    p.drawLine(routeX, rb2Bottom, routeX, mergeY);
    p.drawLine(routeX, mergeY, centerX, mergeY);
    QPolygon arrowLeft;
    arrowLeft << QPoint(centerX, mergeY)
              << QPoint(centerX+10, mergeY-6)
              << QPoint(centerX+10, mergeY+6);
    p.setBrush(FC_ARROW);
    p.drawPolygon(arrowLeft);
    p.setBrush(Qt::NoBrush);

    y = mergeY + 10;

    // 6. Вывод
    drawArrowDown(p, centerX, mergeY, y + bh/2 - 5);
    y += bh/2;
    drawRoundedBlock(p, centerX, y, bw, bh, "Вывод f(x)", FC_OUT_FILL);
    y1 = y + bh/2;
    y += bh + gap;
    drawArrowDown(p, centerX, y1, y - 20);

    // 7. Конец
    drawRoundedBlock(p, centerX, y, 140, 40, "Конец", FC_GREEN_FILL);
}

// ───────────────────────────────────────────────────────────────────
SchemaDialog::SchemaDialog(QWidget *parent)
    : QDialog(parent)
{
    setStyleSheet(QString(
        "QDialog { background-color: #0d1117; color: #e6edf3; font-family: '%1'; }"
        "QScrollArea { background: #0d1117; border: 1px solid #30363d; border-radius: 4px; }"
        "QScrollBar:vertical   { background: #0d1117; width: 8px; border: none; }"
        "QScrollBar::handle:vertical { background: #30363d; border-radius: 4px; }"
        "QScrollBar:horizontal { background: #0d1117; height: 8px; border: none; }"
        "QScrollBar::handle:horizontal { background: #30363d; border-radius: 4px; }"
    ).arg(FONT_FAMILY));
    setupUI();
}

SchemaDialog::~SchemaDialog() {}

void SchemaDialog::setupUI()
{
    setWindowTitle("Блок-схема вычислительного процесса");
    resize(750, 750);
    setMinimumSize(500, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(6);

    canvas = new FlowchartWidget();

    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(canvas);
    scrollArea->setWidgetResizable(false);
    scrollArea->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(scrollArea, 1);

    closeBtn = new QPushButton("Закрыть", this);
    closeBtn->setMinimumHeight(36);
    closeBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #388bfd; color: #ffffff;"
        "  border: 1px solid rgba(240,246,252,0.1); border-radius: 6px;"
        "  font-size: 11pt; padding: 4px 20px;"
        "}"
        "QPushButton:hover { background-color: #58a6ff; }"
    );
    QHBoxLayout *btnRow = new QHBoxLayout();
    btnRow->addStretch(1);
    btnRow->addWidget(closeBtn);
    btnRow->addStretch(1);
    mainLayout->addLayout(btnRow);

    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
}
