#include "tree.h"
void tree::gen_header(ostream &out)
{
    out << "\t.586" << endl;
    out << "\t.model flat, stdcall" << endl;
    out << "\toption casemap :none" << endl;
    out << endl;
    out << "\tinclude \\masm32\\include\\windows.inc" << endl;
    out << "\tinclude \\masm32\\include\\user32.inc" << endl;
    out << "\tinclude \\masm32\\include\\kernel32.inc" << endl;
    out << "\tinclude \\masm32\\include\\masm32.inc" << endl;
    out << endl;
    out << "\tincludelib \\masm32\\lib\\user32.lib" << endl;
    out << "\tincludelib \\masm32\\lib\\kernel32.lib" << endl;
    out << "\tincludelib \\masm32\\lib\\masm32.lib" << endl;
}
void tree::stmt_get_label(Node *t) // ��ÿһ??�ڵ����ɱ�??
{
}
void tree::expr_get_label(Node *t) // ���ɲ������ʽ???��ı��
{
}
void tree::gen_code(ostream &out) // ʹ��??�������ɴ�??
{
}
void tree::get_temp_var(Node *t) // ��ȡ�������??����ʱ��??
{
}
void tree::gen_decl(ostream &out, Node *t) // ����??Ҫд�ڱ������ģ���??????һ��д�ã���Ҫ����???����
{
}
void tree::expr_gen_code(ostream &out, Node *t) // ���ɲ������ʽ???��Ĵ���
{
}
void tree::stmt_gen_code(ostream &out, Node *t) // ���ɱ��ʽ�Ĵ���
{
}

Node::Node()
{
    for (int i = 0; i < 4; i++)
    {
        children[i] = NULL;
    }
    sibing = NULL;
}
Node *generate_expr_node()
{
    Node *ret = NULL;
    ret = new Node();
    ret->nd_type = EXPR_t;
    return ret;
}
Node *generate_stmt_node()
{
    Node *ret = NULL;
    ret = new Node();
    ret->nd_type = STMT_t;
    return ret;
}
Node *generate_null_node()
{
    Node *ret = NULL;
    ret = new Node();
    ret->nd_type = NULL_t;
    return ret;
}
Node *generate_decl_node()
{
    Node *ret = NULL;
    ret = new Node();
    ret->nd_type = DECL_t;
    return ret;
}
Node *generate_const_node()
{
    Node *ret = NULL;
    ret = new Node();
    ret->nd_type = CONS_t;
    return ret;
}
Node *generate_ID_node()
{
    Node *ret = new Node();
    ret->nd_type = ID_t;
    return ret;
}
string generate_expr_code(Node *node1, Node *node2, string op)
{
    string ret = "\tmov eax, " + temp_table[node1->it] + "\n";
    // ret+= + "\tmov ebx, " + temp_table[node2->it] + "\n"
    // ��ʵ??����������ȫ���ü��أ���Ϊ֧��???������ַ����???
    ret += "\t";
    if (op == "-")
    {
        //??����??��???һ�׼Ĵ���, ����??����??��Ҫ֧�ּӼ���???
        ret += "sub eax, " + temp_table[node2->it] + "\n";
    }
    else if (op == "+")
    {
        ret += "add eax, " + temp_table[node2->it] + "\n";
    }
    else if (op == "*")
    {
        ret += "imul eax, " + temp_table[node2->it] + "\n";
    }
    else if (op == "/")
    {
        // �������������edx?
        ret += "mov edx, 0\n\t";
        ret += "mov ebx, " + temp_table[node2->it] + "\n";
        ret += "idiv ebx\n";
    }
    else if (op == "%")
    {
        // �����Ľ��������edx???
        ret += "mov edx, 0\n\t";
        ret += "mov ebx, " + temp_table[node2->it] + "\n";
        ret += "idiv ebx\n";
    }
    else if (op == "^")
    {
        ret += "xor eax, " + temp_table[node2->it] + "\n";
    }
    else if (op == "|")
    {
        ret += "or eax, " + temp_table[node2->it] + "\n";
    }
    else if (op == "&")
    {
        ret += "and eax, " + temp_table[node2->it] + "\n";
    }
    // �����ڵķ���ֵ��ͬ��????Ҫ�޸������
    // ��Ҫ����ڵ�ĵ�ַ
    else if (op == "=")
    {
        // �õ�node1���ڷ��ű��д洢�ı�����
        ret += "mov ebx, " + temp_table[node2->it];
        ret += "mov " + node1->name + ", ebx\n";
    }
    else if (op == ">>=")
    {
        ret += "\tmov ecx, " + temp_table[node2->it];
        ret += "sar eax, cl\n";
        ret += "mov " + node1->name + ", eax\n";
    }
    else if (op == "<<=")
    {
        ret += "\tmov ecx, " + temp_table[node2->it];
        ret += "sal eax, cl\n";
        ret += "mov " + node1->name + ", eax\n";
    }
    else if (op == "+=")
    {
        ret += "add eax, " + temp_table[node2->it] + "\n";
        ret += "mov " + node1->name + ", eax\n";
    }
    else if (op == "-=")
    {
        ret += "sub eax, " + temp_table[node2->it] + "\n";
        ret += "mov " + node1->name + ", eax\n";
    }
    else if (op == "*=")
    {
        ret += "imul eax, " + temp_table[node2->it] + "\n";
        ret += "mov " + node1->name + ", eax\n";
    }
    else if (op == "/=")
    {
        ret += "mov edx, 0\n\t";
        ret += "mov ebx, " + temp_table[node2->it] + "\n";
        ret += "idiv ebx\n";
        ret += "mov " + node1->name + ", eax\n";
    }
    else if (op == "%=")
    {
        ret += "mov edx, 0\n\t";
        ret += "mov ebx, " + temp_table[node2->it] + "\n";
        ret += "idiv ebx\n";
        ret += "mov " + node1->name + ", edx\n";
    }
    else if (op == "&=")
    {
        ret += "and eax, " + temp_table[node2->it] + "\n";
        ret += "mov " + node1->name + ", eax\n";
    }
    else if (op == "^=")
    {
        ret += "xor eax, " + temp_table[node2->it] + "\n";
        ret += "mov " + node1->name + ", eax\n";
    }
    else if (op == "|=")
    {
        ret += "or eax, " + temp_table[node2->it] + "\n";
        ret += "mov " + node1->name + ", eax\n";
    }
    else if (op == ">>")
    {
        ret += "\tmov ecx, " + temp_table[node2->it];
        ret += "sar eax, cl\n";
    }
    else if (op == "<<")
    {
        ret += "\tmov ecx, " + temp_table[node2->it];
        ret += "sal eax, cl\n";
    }
    if (op == "%")
        ret += "\tmov " + temp_table[node1->it] + ", edx\n";
    ret += "\tmov " + temp_table[node1->it] + ", eax\n";
    return ret;
}

string generate_double_code(Node *node1, Node *node2, string op)
{
    // ����??������??
    string ret = "\tfld " + temp_table[node1->it] + "\n";
    if (op == "+")
    {
        ret += "\tfld " + temp_table[node2->it] + "\n\t";
        ret += "fadd\n";
    }
    else if (op == "-")
    {
        ret += "\tfld " + temp_table[node2->it] + "\n\t";
        ret += "fsub\n";
    }
    else if (op == "*")
    {
        ret += "\tfld " + temp_table[node2->it] + "\n\t";
        ret += "fmul\n";
    }
    else if (op == "/")
    {
        ret += "\tfld " + temp_table[node2->it] + "\n\t";
        ret += "fdiv\n";
    }
    else if (op == "=")
    {
        // �Ȱ�node1����, ��ѹ??node2����node2��ֵ����node1
        ret += "fstp " + node1->name + "\n";
        ret += "\tfld " + temp_table[node2->it] + "\n\t";
        ret += "fstp " + node2->name + "\n";
    }
    ret += "\t fstp" + temp_table[node1->it] + "\n";
    return ret;
}

// ǰ׺��Ŀ���???
string generate_pre_code(Node *node, string op)
{
    string ret = "\t mov eax, " + temp_table[node->it] + "\n";
    if (op == "&")
    {
        // ȡ��ַ
    }
    else if (op == "*")
    {
        // ȡ??
    }
    else if (op == "-")
    {
        // ȡ��
        ret += "\tmov ebx, eax\n\tmov eax, 0\n";
        ret += "\tsub eax, ebx\n";
    }
    else if (op == "~")
    {
        // ��λȡ��
        ret +="\tnot eax\n";
    }
    else if (op == "!")
    {
        // �߼�???
        //�Ȳ�ʵ��
    }
    else if (op == "++")
    {
        ret += "\tinc eax\n";
    }
    else if (op == "--")
    {
        ret+= "\tdec eax]\n";
    }
    ret += "\tmov " + temp_table[node->it] + ", eax\n";
    ret += "\tmov " + node->name + ", eax\n";
    return ret;
}
// ��׺��Ŀ���???
string generate_post_code(Node *node, string op)
{
    string ret = "\t mov eax, " + temp_table[node->it] + "\n";
    if (op == "++")
    {
        ret += "\tinc eax\n";
    }
    else if (op == "--")
    {
        ret += "\tdec eax]\n";
    }
    // ��׺���ʽ����֮ǰ��ֵ������
    ret += "\tmov " + node->name + ", eax\n";
    return ret;
}