#ifndef SCHEMADIALOG_H
#define SCHEMADIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QScrollArea>
#include <QWidget>

// ── Inner canvas widget that draws the flowchart ──────────────────────
class FlowchartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FlowchartWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    // Drawing helpers — fill + border colour
    void drawRoundedBlock(QPainter &p, int cx, int cy, int w, int h,
                          const QString &text,
                          const QColor &fill, const QColor &border);
    void drawDiamond(QPainter &p, int cx, int cy, int w, int h,
                     const QString &text,
                     const QColor &fill, const QColor &border);
    void drawArrowDown (QPainter &p, int cx, int y1, int y2);
    void drawArrowRight(QPainter &p, int x1, int x2, int y);
    void drawArrowLine (QPainter &p, int x1, int y1, int x2, int y2);
    void drawText      (QPainter &p, int cx, int cy, int w, int h,
                        const QString &text);
};

// ── The dialog ────────────────────────────────────────────────────────
class SchemaDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SchemaDialog(QWidget *parent = nullptr);
    ~SchemaDialog();

private:
    QPushButton     *closeBtn;
    FlowchartWidget *canvas;
    QScrollArea     *scrollArea;

    void setupUI();
};

#endif // SCHEMADIALOG_H
