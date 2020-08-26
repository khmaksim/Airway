#include <QApplication>
#include "qgroupheaderview.h"

QGroupHeaderView::QGroupHeaderView(Qt::Orientation ornt, QWidget *parent) :
    QHeaderView(ornt, parent)
{
    rowsCount = 1;
    showSizeTip = false;
    checkable = false;
    checked = false;

    connect(this, SIGNAL(sectionCountChanged(int,int)), SLOT(newSectionCount(int,int)));
    connect(this, SIGNAL(sectionResized(int,int,int)), SLOT(onSectionResize(int,int,int)));
}

QSize QGroupHeaderView::sizeHint() const
{
    QSize size = QHeaderView::sizeHint();
    size.rheight() *= rowsCount;
    return size;
}

void QGroupHeaderView::setSizeTip(bool show)
{
    showSizeTip = show;
}

// builds header matrix
void QGroupHeaderView::fillHeaderMatrix()
{
    if(!count()) return;
    headerMatrix.clear();
    headerMatrix.resize(count());
    for(int i = 0; i < count(); i++)
        headerMatrix[i].resize(rowsCount);
    for(int i = 0; i < count(); i++) {
        QStringList hText = model()->headerData(i, Qt::Horizontal).toString()
                .split('|');
        if(hText.length() < rowsCount)
            for(int k = hText.length(); k < rowsCount; k++)
                hText << "";

        // for each header text
        int currentItem(0);
        for(int j = 0; j < rowsCount; j++) {
            if(!i) {
                HeaderStruct hs;
                if(!j) {
                    hs.sectText = hText[0];
                    hs.colspan = hs.rowspan = 1;
                    headerMatrix[i][j] = hs;
                    currentItem = j;
                } else {
                    if(hText[j] != "") {
                        hs.sectText = hText[j];
                        hs.colspan = hs.rowspan = 1;
                        headerMatrix[i][j] = hs;
                        currentItem = j;
                    } else {
                        hs.sectText = "";
                        hs.colspan = hs.rowspan = -1;
                        headerMatrix[i][j] = hs;
                        headerMatrix[i][currentItem].rowspan++;
                    }
                }
            }// 1st column
            else {
                int searchCol = i-1;
                bool isEmpty = false;
                while(searchCol >= 0 && headerMatrix[searchCol][j].sectText == "") {
                    searchCol--;
                }
                if (searchCol >= 0 && headerMatrix[searchCol][j].sectText == hText[j])
                    isEmpty = true;
                HeaderStruct hs;
                if(isEmpty) {
                    hs.sectText = "";
                    hs.colspan = hs.rowspan = -1;
                    headerMatrix[i][j] = hs;
                    headerMatrix[searchCol][j].colspan++;
                } else {
                    if (hText[j] != "") {
                        hs.sectText = hText[j];
                        hs.colspan = hs.rowspan = 1;
                        headerMatrix[i][j] = hs;
                    } else {
                        int searchRow(j-1);
                        while (searchRow >= 0 && headerMatrix[i][searchRow].sectText == "") {
                            searchRow--;
                        }
                        hs.sectText = "";
                        hs.colspan = hs.rowspan = -1;
                        headerMatrix[i][j] = hs;
                        headerMatrix[i][searchRow].rowspan++;
                    }
                }
            }// other columns
        }//j
    }//i
}

// returns row count for header
int QGroupHeaderView::calcRowCount(int rows)
{
    if(!model())
        return 1;
    int rc(1);
    for(int i = 0; i < rows; i++)
        rc = qMax(rc, model()->headerData(i, Qt::Horizontal, Qt::DisplayRole)
                 .toString().split('|').count());
    return rc;
}

void QGroupHeaderView::paintHeader(QPainter *p, int logicalIndex, const QRect &rect)
{
    // some code from original qheaderview.cpp
    QStyleOptionHeader opt;
    initStyleOption(&opt);
    QStyle::State state = QStyle::State_None;
    if (isEnabled())
        state |= QStyle::State_Enabled;
    if (window()->isActiveWindow())
        state |= QStyle::State_Active;
    if (isSortIndicatorShown() && sortIndicatorSection() == logicalIndex) {
        opt.sortIndicator = (sortIndicatorOrder() == Qt::AscendingOrder)
                            ? QStyleOptionHeader::SortDown : QStyleOptionHeader::SortUp;
    }
    // setup the style options structure
    QVariant textAlignment = model()->headerData(logicalIndex, orientation(), Qt::TextAlignmentRole);
    opt.section = logicalIndex;
    opt.state |= state;
    opt.textAlignment = Qt::Alignment(textAlignment.isValid() ? Qt::Alignment(textAlignment.toInt()) : defaultAlignment());
    opt.iconAlignment = Qt::AlignVCenter;
    opt.text = model()->headerData(logicalIndex, orientation(), Qt::DisplayRole).toString();

    if (textElideMode() != Qt::ElideNone)
        opt.text = opt.fontMetrics.elidedText(opt.text, textElideMode(), rect.width() - 4);

    QVariant variant = model()->headerData(logicalIndex, orientation(), Qt::DecorationRole);
    opt.icon = qvariant_cast<QIcon>(variant);
    if (opt.icon.isNull())
        opt.icon = qvariant_cast<QPixmap>(variant);

    QVariant foregroundBrush = model()->headerData(logicalIndex, orientation(), Qt::ForegroundRole);
    if (foregroundBrush.canConvert<QBrush>())
        opt.palette.setBrush(QPalette::ButtonText, qvariant_cast<QBrush>(foregroundBrush));

    QPointF oldBO = p->brushOrigin();
    QVariant backgroundBrush = model()->headerData(logicalIndex, orientation(),
                                                    Qt::BackgroundRole);
    if (backgroundBrush.canConvert<QBrush>()) {
        opt.palette.setBrush(QPalette::Button, qvariant_cast<QBrush>(backgroundBrush));
        opt.palette.setBrush(QPalette::Window, qvariant_cast<QBrush>(backgroundBrush));
        p->setBrushOrigin(opt.rect.topLeft());
    }

    // the section position
    int visual = visualIndex(logicalIndex);
    Q_ASSERT(visual != -1);
    if (count() == 1)
        opt.position = QStyleOptionHeader::OnlyOneSection;
    else if (visual == 0)
        opt.position = QStyleOptionHeader::Beginning;
    else if (visual == count() - 1)
        opt.position = QStyleOptionHeader::End;
    else
        opt.position = QStyleOptionHeader::Middle;
    opt.orientation = orientation();


    for (int i = 0; i < rowsCount; i++) {
        if (headerMatrix[logicalIndex][i].sectText == "")
            continue;
        int wd(0), hg(0);
        for (int j = 0; j < headerMatrix[logicalIndex][i].colspan; j++)
            wd += sectionSize(logicalIndex+j);
        for (int j = 0; j < headerMatrix[logicalIndex][i].rowspan; j++)
            hg++;
        hg *= rect.height()/rowsCount;
        opt.text = headerMatrix[logicalIndex][i].sectText;
        opt.rect = QRect(rect.left(), rect.top()+i*(rect.height()/rowsCount), wd, hg);
        style()->drawControl(QStyle::CE_Header, &opt, p, this);
    }

    if (logicalIndex == 0 && isCheckable()) {
        QStyleOptionButton option;
        QRect cr = qApp->style()->subElementRect(QStyle::SE_ViewItemCheckIndicator, &option);
        int deltaX = (rect.width() - cr.width()) / 2;
        int deltaY = (rect.height() - cr.height()) / 2;
        option.rect = QRect(rect.left() + deltaX, rect.top() + deltaY, cr.width(), cr.height());
        option.state |= QStyle::State_Enabled;

        if (isChecked())
            option.state |= QStyle::State_On;
        else
            option.state |= QStyle::State_Off;

        style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &option, p);
    }
    p->setBrushOrigin(oldBO);
}

void QGroupHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    Q_UNUSED(painter)
    Q_UNUSED(rect)
    Q_UNUSED(logicalIndex)
    /*Drawing header in this way may cause some sort of 'bugs',
     *so this method just ignored and 'moved' to paintEvent().
     */
}

void QGroupHeaderView::paintEvent(QPaintEvent *e)
{
    QHeaderView::paintEvent(e); // processing default event
    QPainter p(viewport());
    p.translate(-offset(), 0);  // offset if model view has been scrolled

    for(int i = 0; i < count(); i++) {
        QRect r(sectionPosition(i), 0, sectionSize(i), height());
        paintHeader(&p, i, r);
    }
}

void QGroupHeaderView::newSectionCount(int, int newcount)
{
    rowsCount = calcRowCount(newcount);
    fillHeaderMatrix();
}

void QGroupHeaderView::onSectionResize(int, int, int newSize)
{
    if(showSizeTip)
        QToolTip::showText(QCursor::pos(), QString::number(newSize));
    reset();
}

void QGroupHeaderView::setTextSectionColor(int section, int row, const QColor &color)
{
    Q_UNUSED(color)
    if (section > count() -1 || row > rowsCount - 1)
          return;
//     headerMatrix[section][row].palette.setColor(QPalette::ButtonText, color);
}

bool QGroupHeaderView::isCheckable() const
{
    return checkable;
}

void QGroupHeaderView::setCheckable(bool checkable)
{
    this->checkable = checkable;
}

bool QGroupHeaderView::isChecked() const
{
    return checked;
}

void QGroupHeaderView::setChecked(bool checked)
{
    this->checked = checked;
    updateSection(0);
}

void QGroupHeaderView::mousePressEvent(QMouseEvent *event)
{
    // if mouse press on first column
    if (event->pos().x() <= sectionSize(0)) {
        int height = sizeHint().height();
        int width = sectionSize(0);

        QStyleOptionButton option;
        QRect cr = qApp->style()->subElementRect(QStyle::SE_ViewItemCheckIndicator, &option);
        int deltaX = (width - cr.width()) / 2;
        int deltaY = (height - cr.height()) / 2;
        option.rect = QRect(deltaX, deltaY, cr.width(), cr.height());
        if (option.rect.contains(event->pos())) {
            //    if (event->pos().x() <= sectionSize(0)) {
            if (isChecked())
                checked = false;
            else
                checked = true;

            emit clickedCheckBox(checked);
            event->accept();
        }
    }
    QHeaderView::mousePressEvent(event);
}
