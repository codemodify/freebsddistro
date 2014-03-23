#ifndef TABSETTINGS_H
#define TABSETTINGS_H

class QTextBlock;

struct TabSettings
{
    TabSettings();

//    void toSettings(const QString &category, QSettings *s) const;
//    void fromSettings(const QString &category, const QSettings *s);


    int lineIndentPosition(const QString &text) const;
    int firstNonSpace(const QString &text) const;
    int columnAt(const QString &text, int position) const;
    int spacesLeftFromPosition(const QString &text, int position) const;
    int indentedColumn(int column, bool doIndent = true) const;
    QString indentationString(int startColumn, int targetColumn) const;

    void indentLine(QTextBlock block, int newIndent) const;

    int trailingWhitespaces(const QString &text) const;
    bool isIndentationClean(const QString &text) const;


    bool m_spacesForTabs;
    bool m_autoIndent;
    bool m_smartBackspace;
    int m_tabSize;
    int m_indentSize;

    bool equals(const TabSettings &ts) const;
};

inline bool operator==(const TabSettings &t1, const TabSettings &t2) { return t1.equals(t2); }
inline bool operator!=(const TabSettings &t1, const TabSettings &t2) { return !t1.equals(t2); }

#endif
