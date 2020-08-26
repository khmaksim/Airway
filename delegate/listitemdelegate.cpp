#include "listitemdelegate.h"

ListItemDelegate::ListItemDelegate(QStyledItemDelegate *parent)
{
    Q_UNUSED(parent)
}

ListItemDelegate::~ListItemDelegate()
{
}

QSize ListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!index.isValid())
        return QSize();

    QString codeICAO = index.data(CodeICAORole).toString();
    QString nameAirfield = index.data(NameAirfieldRole).toString();

    QFont codeICAOFont = QApplication::font();
    codeICAOFont.setBold(true);
    QFont nameAirfieldFont = QApplication::font();
    QFontMetrics codeICAOFm(codeICAOFont);
    QFontMetrics nameAirfieldFm(nameAirfieldFont);

    QRect codeICAORect = codeICAOFm.boundingRect(0, 0, option.rect.width(), 0, Qt::AlignLeft|Qt::AlignTop|Qt::TextWordWrap,
                                             codeICAO);
    QRect nameAirfieldRect = nameAirfieldFm.boundingRect(0, 0, option.rect.width(), 0, Qt::AlignLeft|Qt::AlignTop|Qt::TextWordWrap,
                                                   nameAirfield);

    QSize size(option.rect.width(), codeICAORect.height() + nameAirfieldRect.height() + 6);

    return size;
}

void ListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!index.isValid())
           return;

       painter->save();

       if (option.state & QStyle::State_Selected) {
           painter->fillRect(option.rect, option.palette.highlight());
           painter->setPen(Qt::white);
       }
       else
           painter->setPen(Qt::black);

       QString codeICAO = index.data(CodeICAORole).toString();
       QString nameAirfield = index.data(NameAirfieldRole).toString();

       QFont codeICAOFont = QApplication::font();
       codeICAOFont.setBold(true);
       QFont nameAirfieldFont = QApplication::font();
       QFontMetrics codeICAOFm(codeICAOFont);
       QFontMetrics nameAirfieldFm(nameAirfieldFont);

       QRect codeICAORect = codeICAOFm.boundingRect(option.rect.left(), option.rect.top() + 2,
                                                    option.rect.width(), 0,
                                                    Qt::AlignLeft|Qt::AlignTop|Qt::TextWordWrap,
                                                    codeICAO);
       QRect nameAirfieldRect = nameAirfieldFm.boundingRect(codeICAORect.left(), codeICAORect.bottom() + 2,
                                                            option.rect.width(), 0,
                                                            Qt::AlignLeft|Qt::AlignTop|Qt::TextWordWrap,
                                                            nameAirfield);



       painter->setFont(codeICAOFont);
       painter->drawText(codeICAORect, Qt::AlignLeft|Qt::AlignTop|Qt::TextWordWrap, codeICAO);
       painter->setFont(nameAirfieldFont);
       painter->drawText(nameAirfieldRect, Qt::AlignLeft|Qt::AlignTop|Qt::TextWordWrap, nameAirfield);

       painter->restore();
       QStyledItemDelegate::paint(painter, option, index);
}
