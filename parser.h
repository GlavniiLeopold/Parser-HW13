#ifndef parser_H
#define parser_H

#include <QVector>
#include <QString>
#include <iostream>

struct Connection
{
    Connection(int o, QString r, int f1, int f2){rule = r; old = o; from1 = f1; from2 = f2;}
    QString rule;
    int old;
    int from1;
    int from2;
};


struct Rule
{
    Rule(QString r, QString f)
    {
        result = r;
        from = f.split(' ');
        info = f.toStdString();
        size = from.size();
    }
    QString result;
    QStringList from;
    std::string info;
    int size;
};


class Tree
{
    QVector<Connection*> tree;
    std::string val(int node, int &from1, int &from2){
        for(int i=0; i<tree.size(); i++)
        {
            if(tree.at(i)->old == node)
            {
                from1 = tree.at(i)->from1;
                from2 = tree.at(i)->from2;
                return tree.at(i)->rule.toStdString();
            }
        }
        from1 = from2 = -1;
        return "X";
    }
public:
    Tree(QVector<Connection*> tree){this->tree = tree;}

    void print(int node, int pos = 0)
    {
        int right, left;
        if (node==-1) {
            for(int i = 0; i < pos; ++i)
                std::cout << "    ";
            std::cout << '-' << std::endl;
            return;
        }
        std::string key = val(node, right, left);
        print(right,pos+1);
        for(int i = 0; i < pos; i++)
            std::cout << "    ";
        std::cout << key << std::endl;
        print(left,pos+1);
    }

};



class Parser
{
public:
    Parser();

    void print_tree_nodes();
    void add_rule(QString r, QString f);
    void parse(QString data);

private:

    QVector<QString> punctuation;
    QVector<QString> keywords;
    QVector<QString> op;
    QVector<Rule*> rules;
    QVector<QStringList*> matrix;
    QVector<Connection*> tree;

    void pretty_printer(QStringList terms, QStringList words);
    void delete_redundant_terms(QStringList &terms, QStringList &words);
    void analyse_synt(QStringList &words);
    QString check_rule(QString rule0, QString rule1);
    QString check_rule(QString rule0);
    void get_diagonals(int pos, int size);
    void analyse_lex(QStringList &words);
    void divide(QStringList &words, int pos);
    inline bool is_num(QChar n);
    inline int is_op(QChar a, QChar b);
    inline void push_back(QVector<QString> &v, QString s);
};

#endif // parser_H
