#ifndef QGROUPHEADERVIEW_H
#define QGROUPHEADERVIEW_H

#include <QHeaderView>
#include <QToolTip>
#include <QPainter>
#include <QBrush>
#include <QPaintEvent>
#include <QDebug>
#include <QEvent>

class QGroupHeaderView : public QHeaderView
{
        Q_OBJECT
    public:
        explicit QGroupHeaderView(Qt::Orientation ornt = Qt::Horizontal, QWidget *parent = 0);
        QSize sizeHint() const;
        void setSizeTip(bool show);
        void setCheckable(bool);
        bool isCheckable() const;
        void setChecked(bool);
        bool isChecked() const;

    private:
        // header structure
        struct HeaderStruct {
                QString sectText;
                int colspan;
                int rowspan;

                //include <QDebug> for debugging
                //        friend QDebug operator <<(QDebug d, const HeaderStruct &s) {
                //            return d << "HS{" << s.sectText << s.colspan << s.rowspan << "}";
                //        }
        };
        bool checked;
        bool checkable;
        int rowsCount;
        bool showSizeTip;
        QVector<QVector<HeaderStruct> > headerMatrix; // data for cell paiting

        void fillHeaderMatrix();
        int calcRowCount(int rows);
        void paintHeader(QPainter *p, int logicalIndex, const QRect &rect);
        void setTextSectionColor(int section, int row, const QColor &color);

    protected:
        void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;
        void paintEvent(QPaintEvent *e);
        void mousePressEvent(QMouseEvent *event);

    private slots:
        void newSectionCount(int, int newcount);
        void onSectionResize(int, int, int newSize);

    signals:
        void clickedCheckBox(bool);
};

#endif // QGROUPHEADERVIEW_H
