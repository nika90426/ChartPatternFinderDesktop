#ifndef INSTRUMENTLISTTABLEVIEW_H
#define INSTRUMENTLISTTABLEVIEW_H

#include <QTableView>
#include <QDir>

class InstrumentListTableView : public QTableView
{
    Q_OBJECT

private:
    QStringList fileList_;
    QDir dir_;
public:
    explicit InstrumentListTableView();
    void populateFromCSVFiles(QString quoteFilePath);

public slots:
    void instrumentSelectionChanged (const QItemSelection  &selected,
                                          const QItemSelection  & );
signals:
    void instrumentSelected(const QString &instrumentPath);

public slots:

};

#endif // INSTRUMENTLISTTABLEVIEW_H