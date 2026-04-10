#include "schemadialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QScrollArea>
#include <QPen>
#include <QFont>
#include <QPolygon>

// ═══════════════════════════════════════════════════════════════════════
// FlowchartWidget
// ═══════════════════════════════════════════════════════════════════════

FlowchartWidget::FlowchartWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(700, 1000);
    setStyleSheet("background: white;");
}

// ── Drawing helpers ───────────────────────────────────────────────────

void FlowchartWidget::drawRoundedBlock(QPainter &p, int cx, int cy, int w, int h,
                                        const QString &text, const QColor &fill)
{
    QRect rect(cx - w/2, cy - h/2, w, h);
    QPainterPath path;
    path.addRoundedRect(rect, 10, 10);
    p.fillPath(path, fill);
    p.setPen(QPen(Qt::black, 2));
    p.drawPath(path);
    p.setPen(Qt::black);
    QFont f = p.font();
    f.setPointSize(10);
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
    p.setPen(QPen(Qt::black, 2));
    p.drawPath(path);
    p.setPen(Qt::black);
    QFont f = p.font();
    f.setPointSize(9);
    p.setFont(f);
    QRect textRect(cx - w/2 + 15, cy - h/2 + 10, w - 30, h - 20);
    p.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, text);
}

void FlowchartWidget::drawArrowDown(QPainter &p, int cx, int y1, int y2)
{
    p.setPen(QPen(Qt::black, 2));
    p.drawLine(cx, y1, cx, y2);
    // Arrowhead
    QPolygon arrow;
    arrow << QPoint(cx, y2) << QPoint(cx - 6, y2 - 10) << QPoint(cx + 6, y2 - 10);
    p.setBrush(Qt::black);
    p.drawPolygon(arrow);
    p.setBrush(Qt::NoBrush);
}

void FlowchartWidget::drawArrowRight(QPainter &p, int x1, int x2, int y)
{
    p.setPen(QPen(Qt::black, 2));
    p.drawLine(x1, y, x2, y);
    QPolygon arrow;
    arrow << QPoint(x2, y) << QPoint(x2 - 10, y - 6) << QPoint(x2 - 10, y + 6);
    p.setBrush(Qt::black);
    p.drawPolygon(arrow);
    p.setBrush(Qt::NoBrush);
}

void FlowchartWidget::drawArrowLine(QPainter &p, int x1, int y1, int x2, int y2)
{
    p.setPen(QPen(Qt::black, 2));
    p.drawLine(x1, y1, x2, y2);
}

void FlowchartWidget::drawText(QPainter &p, int cx, int cy, int w, int h, const QString &text)
{
    QRect rect(cx - w/2, cy - h/2, w, h);
    QFont f = p.font();
    f.setPointSize(9);
    f.setBold(true);
    p.setFont(f);
    p.setPen(Qt::black);
    p.drawText(rect, Qt::AlignCenter, text);
    f.setBold(false);
    p.setFont(f);
}

// ── Paint the flowchart ──────────────────────────────────────────────

void FlowchartWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QColor greenFill(200, 240, 200);    // start/end
    QColor blueFill(200, 220, 255);     // process
    QColor yellowFill(255, 255, 200);   // decision
    QColor outputFill(200, 240, 230);   // output

    int centerX = 220;   // main flow center
    int rightX  = 520;   // right branch for "Да" results
    int bw = 200;        // block width
    int bh = 45;         // block height
    int dw = 240;        // diamond width
    int dh = 80;         // diamond height
    int gap = 30;        // vertical gap between elements

    int y = 40;

    // ── 1. Начало ──
    drawRoundedBlock(p, centerX, y, 140, 40, "Начало", greenFill);
    int y1 = y + 20;
    y += 40 + gap;
    drawArrowDown(p, centerX, y1, y - bh/2);

    // ── 2. Ввод x, a, b, c ──
    drawRoundedBlock(p, centerX, y, bw, bh, "Ввод x, a, b, c", blueFill);
    y1 = y + bh/2;
    y += bh + gap;
    drawArrowDown(p, centerX, y1, y - dh/2);

    // ── 3. x < -2 ? ──
    int diamondY1 = y;
    drawDiamond(p, centerX, y, dw, dh, "x < -2 ?", yellowFill);

    // "Да" → right
    drawArrowRight(p, centerX + dw/2, rightX - bw/2, y);
    drawText(p, centerX + dw/2 + 25, y - 14, 40, 20, "Да");

    // Right branch: f = |x·a| - 2
    drawRoundedBlock(p, rightX, y, bw, bh, "f = |x·a| − 2", QColor(255, 200, 200));

    // "Нет" → down
    y1 = y + dh/2;
    y += dh + gap;
    drawArrowDown(p, centerX, y1, y - dh/2);
    drawText(p, centerX + 16, y1 + 10, 40, 20, "Нет");

    // ── 4. -2 ≤ x < 2 ? ──
    int diamondY2 = y;
    drawDiamond(p, centerX, y, dw, dh, "-2 ≤ x < 2 ?", yellowFill);

    // "Да" → right
    drawArrowRight(p, centerX + dw/2, rightX - bw/2, y);
    drawText(p, centerX + dw/2 + 25, y - 14, 40, 20, "Да");

    // Right branch: f = b·x² + x + 1
    drawRoundedBlock(p, rightX, y, bw, bh, "f = b·(x²) + x + 1", QColor(200, 255, 200));

    // "Нет" → down
    y1 = y + dh/2;
    y += dh + gap;
    drawArrowDown(p, centerX, y1, y - bh/2);
    drawText(p, centerX + 16, y1 + 10, 40, 20, "Нет");

    // ── 5. f = |x-2| + 1·c ──
    drawRoundedBlock(p, centerX, y, bw, bh, "f = |x − 2| + 1·c", QColor(200, 200, 255));
    y1 = y + bh/2;

    // ── Now merge all three paths to "Вывод f(x)" ──
    // Right branches need to come down to join
    int mergeY = y + bh + gap + 20;

    // From center block (branch 3) → down to merge
    drawArrowDown(p, centerX, y1, mergeY - 10);

    // From right branch 1 (at diamondY1): draw down
    drawArrowLine(p, rightX, diamondY1 + bh/2, rightX, mergeY - 10);

    // From right branch 2 (at diamondY2): draw down  
    // Already same X, just draw vertical to merge
    // They share rightX, so draw from branch2 bottom to mergeY
    // Actually branch1 and branch2 are at different Y, both need to reach mergeY
    // Branch 1 is at diamondY1, branch 2 is at diamondY2
    // Draw down from branch1 block
    int rb1Bottom = diamondY1 + bh/2;
    int rb2Bottom = diamondY2 + bh/2;

    // Draw vertical line from rightX down to mergeY
    p.setPen(QPen(Qt::black, 2));
    p.drawLine(rightX, rb1Bottom, rightX, mergeY - 10);

    // Draw horizontal line from rightX to centerX at mergeY-10
    p.drawLine(rightX, mergeY - 10, centerX, mergeY - 10);

    y = mergeY + 15;

    // ── 6. Вывод f(x) ──
    drawRoundedBlock(p, centerX, y, bw, bh, "Вывод f(x)", outputFill);
    y1 = y + bh/2;
    y += bh + gap;
    drawArrowDown(p, centerX, y1, y - 20);

    // ── 7. Конец ──
    drawRoundedBlock(p, centerX, y, 140, 40, "Конец", greenFill);
}

// ═══════════════════════════════════════════════════════════════════════
// SchemaDialog
// ═══════════════════════════════════════════════════════════════════════

SchemaDialog::SchemaDialog(QWidget *parent)
    : QDialog(parent)
{
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
    scrollArea->setStyleSheet("QScrollArea { border: 1px solid #ccc; }");
    mainLayout->addWidget(scrollArea, 1);

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
}
