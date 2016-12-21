#include "parser.h"

struct {
    bool operator()(Connection* a, Connection* b)
    {
        return a->old < b->old;
    }
} customSort;

Parser::Parser()
{
    push_back(punctuation, ";,(,)");
    push_back(keywords, "skip,write,read,while,do,if,then,else,fi,od,:=");
    push_back(op, "==,!=,>=,<=,&&,||,:=,>,<,+,−,*,/,%");

    rules.push_back(new Rule("E", "X"));
    rules.push_back(new Rule("E", "N"));
    rules.push_back(new Rule("S", "skip"));
    rules.push_back(new Rule("S", "write E"));
    rules.push_back(new Rule("S", "read E"));

    rules.push_back(new Rule("E", "( B1"));
    rules.push_back(new Rule("B1", "E )"));

    rules.push_back(new Rule("E", "E E1"));
    rules.push_back(new Rule("E1", "OP E"));

    rules.push_back(new Rule("S", "S S1"));
    rules.push_back(new Rule("S1", "; S"));

    rules.push_back(new Rule("S", "S ;"));

    rules.push_back(new Rule("S", "X S2"));
    rules.push_back(new Rule("S2", ":= E"));

    rules.push_back(new Rule("S", "while W1"));
    rules.push_back(new Rule("W1", "E W2"));
    rules.push_back(new Rule("W2", "do W3"));
    rules.push_back(new Rule("W3", "S od"));

    rules.push_back(new Rule("S", "if I1"));
    rules.push_back(new Rule("I1", "E I2"));
    rules.push_back(new Rule("I2", "then I3"));
    rules.push_back(new Rule("I3", "S I4"));
    rules.push_back(new Rule("I4", "else I5"));
    rules.push_back(new Rule("I5", "S fi"));
}

void Parser::add_rule(QString r, QString f)
{
    rules.push_back(new Rule(r, f));
}

void Parser::delete_redundant_terms(QStringList &terms, QStringList &words)
{
    for(int i=0; i<words.size()-2; i++)
    {
        QString term = terms.at(i);
        QString second_word = words.at(i+1);
        QString third_word = words.at(i+2);
        //QString second_term = terms.at(i+1);
        QString third_term = terms.at(i+2);
        if(term == "(" && third_term == ")")
        {
            terms.removeAt(i+2);
            terms.removeAt(i);
            words.removeAt(i+2);
            words.removeAt(i);
            i=0;
        }
        if(term == "N")
        {
            if(second_word == "+" || third_term == "N")
            {
                words[i] = QString::number(words.at(i).toInt() + words.at(i+2).toInt());
                terms.removeAt(i+2);
                terms.removeAt(i+1);
                words.removeAt(i+2);
                words.removeAt(i+1);
                i=0;
            }
            if(words.at(i) == "0")
            {
                if(second_word == "&&")
                {
                    if(third_term == "X" || third_term == "N")
                    {
                        terms.removeAt(i+2);
                        terms.removeAt(i+1);
                        words.removeAt(i+2);
                        words.removeAt(i+1);
                        i=0;
                    }
                }
                if(second_word == "||")
                {
                    if(third_term == "X" || third_term == "N")
                    {
                        terms.removeAt(i);
                        terms.removeAt(i+1);
                        words.removeAt(i);
                        words.removeAt(i+1);
                        i=0;
                    }
                }
            }
            if(words.at(i) == "1")
            {
                if(second_word == "&&")
                {
                    if(third_term == "X" || third_term == "N")
                    {
                        terms.removeAt(i);
                        terms.removeAt(i+1);
                        words.removeAt(i);
                        words.removeAt(i+1);
                        i=0;
                    }
                }
                if(second_word == "||")
                {
                    if(third_term == "X" || third_term == "N")
                    {
                        terms.removeAt(i+2);
                        terms.removeAt(i+1);
                        words.removeAt(i+2);
                        words.removeAt(i+1);
                        i=0;
                    }
                }
            }

        }
        if(term == "X" || term == "N")
        {
            if(second_word == "*")
            {
                if(third_word == "0")
                {
                    words[i] = "0";
                    terms[i] = "N";
                    terms.removeAt(i+2);
                    terms.removeAt(i+1);
                    words.removeAt(i+2);
                    words.removeAt(i+1);
                    i=0;
                }
                if(third_word == "1")
                {
                    terms.removeAt(i+2);
                    terms.removeAt(i+1);
                    words.removeAt(i+2);
                    words.removeAt(i+1);
                    i=0;
                }
            }
            else
            {
                if(second_word == "+" && third_word == "0")
                {
                    terms.removeAt(i+2);
                    terms.removeAt(i+1);
                    words.removeAt(i+2);
                    words.removeAt(i+1);
                    i=0;
                }
                else
                {
                    if(second_word == "||")
                    {
                        if(third_word == "0")
                        {
                            terms.removeAt(i+2);
                            terms.removeAt(i+1);
                            words.removeAt(i+2);
                            words.removeAt(i+1);
                            i=0;
                        }
                        if(third_word == "1")
                        {
                            words[i] = "1";
                            terms[i] = "N";
                            terms.removeAt(i+2);
                            terms.removeAt(i+1);
                            words.removeAt(i+2);
                            words.removeAt(i+1);
                            i=0;
                        }
                    }
                    else
                    {
                        if(second_word == "&&")
                        {
                            if(third_word == "0")
                            {
                                words[i] = "0";
                                terms[i] = "N";
                                terms.removeAt(i+2);
                                terms.removeAt(i+1);
                                words.removeAt(i+2);
                                words.removeAt(i+1);
                                i=0;
                            }
                            if(third_word == "1")
                            {
                                terms.removeAt(i+2);
                                terms.removeAt(i+1);
                                words.removeAt(i+2);
                                words.removeAt(i+1);
                                i=0;
                            }
                        }
                    }
                }
            }
        }
    }
}

void Parser::parse(QString data)
{
    tree.clear();
    data = data.replace("	", " ").replace("\\n", " ");
    std::cout << "parsing string >> " << data.toStdString() << "\n";
    for(int i=0; i<data.size(); i++)
    {
        if(punctuation.contains(data.at(i)))
        {
            QString replacement = data.at(i);
            replacement = " " + replacement + " ";
            data.replace(i, 1, replacement);
            i+=2;
        }
    }
    for(int i=0; i<data.size()-1; i++)
    {
        int num = is_op(data.at(i), data.at(i+1));
        if(num==1)
        {
            QString replacement = data.at(i);
            replacement = " " + replacement + " ";
            data.replace(i, 1, replacement);
            i+=2;
        }
        if(num==2)
        {
            QString replacement = data.at(i);
            replacement.append(data.at(i+1));
            replacement = " " + replacement + " ";
            data.replace(i, 2, replacement);
            i+=3;
        }
    }

    QStringList terms = data.split(' ');

    for(int i=0; i<terms.size(); i++)
    {
        if(terms.at(i) == NULL)
        {
            terms.removeAt(i);
            i--;
        }
    }
    QStringList words = terms;

    analyse_lex(terms);

    //продолжение
    delete_redundant_terms(terms, words);
    pretty_printer(terms, words);
    for(int i=0; i<terms.size(); i++)
        tree.push_back(new Connection(words.size()*(words.size()-1)+i, words.at(i), -1, -1));

    analyse_synt(terms);
}

void Parser::pretty_printer(QStringList terms, QStringList words)
{
    std::cout << "\nPRETTY PRINT:\n";
    QString prefix = "";
    QString line = "";
    for(int i=0; i<words.size(); i++)
    {
        if(words.at(i) == "else" || words.at(i) == "do"
                || words.at(i) == "then" || words.at(i) == "od"
                || words.at(i) == "fi")
        {
            if(!line.isEmpty())
                std::cout << (prefix  + line).toStdString() + "\n";
            prefix.chop(3);
            line = words.at(i) + "\n";
            std::cout << (prefix  + line).toStdString();
            line = "";
            if(words.at(i) != "od" && words.at(i) != "fi")
                prefix.append("   ");
        }
        else
        {
            line += words.at(i) + " ";
            if(words.at(i) == ";")
            {
                std::cout << (prefix + line).toStdString() + "\n";
                line = "";
            }
            if(words.at(i) == "if" || words.at(i) == "while")
            {

                std::cout << (prefix + line).toStdString() + "\n";
                prefix.append("   ");
                line = "";
            }
        }
    }
    std::cout << (prefix + line).toStdString();
    return;
}

void Parser::analyse_synt(QStringList &words)
{
    if(matrix.size()!=0)
        matrix.clear();
    int matrix_size = words.size()*words.size();
    for(int i=0; i<matrix_size; i++)
    {
        matrix.push_back(new QStringList());
    }
    //1-уровневые
    for(int i=0; i<words.size(); i++)
    {
        QString rule = check_rule(words.at(i));
        matrix[i+matrix_size-words.size()]->push_back(words.at(i));
        if(rule != NULL)
        {
            matrix[i+matrix_size-words.size()]->push_back(rule);
        }
    }

    //2-уровневые
    for(int i=words.size()-1; i>0; i--)
    {
        for(int j=0; j<i; j++)
        {
            int num = (i-1)*words.size()+j;
            get_diagonals(num, words.size());
        }
    }
    for(int i=0; i<words.size(); i++)
    {
        matrix.push_back(new QStringList(words.at(i)));
    }
    std::sort(tree.begin(), tree.end(), customSort);
    if(matrix.at(0)->contains("S"))
        std::cout << "\n\n!RIGHT!\n\n";
    else
        std::cout << "\n\n!WRONG!\n\n";
}

QString Parser::check_rule(QString rule0, QString rule1)
{
    for(int i=0; i<rules.size(); i++)
    {
        if(rules.at(i)->from.size() == 2)
        {
            if(rules.at(i)->from.first() == rule0 && rules.at(i)->from.last() == rule1)
                return rules.at(i)->result;
        }
    }
    return NULL;
}
QString Parser::check_rule(QString rule0)
{
    for(int i=0; i<rules.size(); i++)
    {
        if(rules.at(i)->from.size() == 1)
        {
            if(rules.at(i)->from.first() == rule0)
                return rules.at(i)->result;
        }
    }
    return NULL;
}

void Parser::get_diagonals(int pos, int size)
{
    int it = 1;
    int a = pos/size+1;
    for(int i=size-a; i>0; i--)
    {
        int first = pos+i*size;
        int second = pos+it*size+it;
        it++;
        for(int j=0; j<matrix[first]->size(); j++)
        {
            for(int k=0; k<matrix[second]->size(); k++)
            {
                QString rule_res = check_rule(matrix[first]->at(j), matrix[second]->at(k));
                if(rule_res != NULL && !matrix[pos]->contains(rule_res))
                {
                    matrix[pos]->push_back(rule_res);
                    tree.push_back(new Connection(pos, rule_res, first, second));
                }
            }
        }
    }
}

void Parser::analyse_lex(QStringList &words)
{
    for(int i=0; i<words.size(); i++)
    {
        if(keywords.contains(words.at(i)))
            std::cout << " keyword >> " << words.at(i).toStdString() <<"\n";
        else
        {
            if(op.contains(words.at(i)))
            {
                std::cout << " operator >> " << words.at(i).toStdString() <<"\n";
                words.replace(i, "OP");
            }
            else
            {
                if( punctuation.contains(words.at(i)))
                    std::cout << " punctuation >> " << words.at(i).toStdString() <<"\n";
                else
                {
                    divide(words, i);
                    if(is_num(words.at(i).at(0)))
                    {
                        std::cout << " number >> " << words.at(i).toStdString() <<"\n";
                        words.replace(i, "N");
                    }
                    else
                    {
                        std::cout << " variable >> " << words.at(i).toStdString() <<"\n";
                        words.replace(i, "X");
                    }
                }
            }
        }
    }
}
void Parser::divide(QStringList &words, int pos)
{
    if(words.at(pos).at(0) >= '0' && words.at(pos).at(0) <= '9')
    {
        int word_size = words.at(pos).size();
        for(int i=1; i<word_size; i++)
        {
            if(!is_num(words.at(pos).at(i)))
            {
                words.insert(pos+1, words.at(pos).right(word_size-i));
                words.replace(pos, words.at(pos).left(i));
                divide(words, pos+1);
                return;
            }
        }
    }
}

void Parser::print_tree_nodes()
{
    Tree *parse_tree = new Tree(tree);
    parse_tree->print(0);

    for(int i=0; i<tree.size(); i++)
    {
        int f1 = tree.at(i)->from1;
        int f2 = tree.at(i)->from2;
        std::cout << "node [" << tree.at(i)->rule.toStdString() << "] in table at >" << tree.at(i)->old << "<\n";
        if(f1 != -1)
        {
            std::cout << "  derived from table position " << f1 << "-\"" << matrix.at(f1)->first().toStdString() << "\"";
            if(f2 != -1)
                std::cout << " and " << f2 << "-\"" << matrix.at(f2)->last().toStdString() << "\"\n";
            else
                std::cout << "\n";
        }
    }
}

inline bool Parser::is_num(QChar n)
{
    if(n >= '0' && n <= '9')
        return 1;
    else
        return 0;
}
inline int Parser::is_op(QChar a, QChar b)
{
    QString temp1 = a;
    QString temp2 = a;
    temp2.append(b);
    for(int i=0; i<op.size(); i++)
    {
        if(op.at(i) == temp2)
            return 2;
        if(op.at(i) == temp1)
            return 1;
    }
    return 0;
}
inline void Parser::push_back(QVector<QString> &v, QString s)
{
    int i = 0;
    QString temp_string = s.section(',', i, i);
    while(temp_string != NULL)
    {
        v.push_back(temp_string);
        i++;
        temp_string = s.section(',', i, i);
    }
}

