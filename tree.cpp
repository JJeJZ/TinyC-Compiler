#include "tree.h"

Node::Node()
{
    for (int i = 0; i < 4; i++)
    {
        children[i] = NULL;
    }
    sibing = NULL;
    state = Valid;
    v_type = None;
    it = -1;
    // valueֻ�ܸ�ֵһ�Σ��Ͳ�����ʼ����
}
Node *check_type(char *id)
{
    Node *ret = generate_ID_node();
    string name = id;
    auto temp_search = ID_Table.find(name);
    if (temp_search != ID_Table.end())
    {
        string type = temp_search->second;
        if (type == "VAR")
        {
            auto search = Var_Table.find(name);
            VarEntry result = search->second;
            ret->v_type = result.type;
            ret->name = id;
        }
        else if (type == "STRUCT")
        {
            auto search = Struct_Table.find(name);
            auto result = search->second;
        }
        else if (type == "FUNC")
        {
            auto search = Fuction_Table.find(name);
            auto result = search->second;
        }
        else if (type == "POINTER")
        {
            auto search = Pointer_Table.find(name);
            auto result = search->second;
        }
    }
    else
    {
        ret->v_type = None;
        ret->name = id;
        ret->state = Not_Def;
    }
    return ret;
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
    string ret;
    string op1; // op1 �������Ǳ�ʶ���ͳ�����û����ʱ����
    if (node1->it == -1)
    {
        if (node1->nd_type == ID_t)
            op1 = node1->name;
        else
            op1 = to_string(node1->value.fvalue);
    }
    else
    {
        op1 = temp_table[node1->it];
        temp_top--;
    }
    ret += "\tmov eax, " + op1 + "\n";
    string op2;
    if (node2->it == -1)
    {
        op2 = to_string(node2->value.fvalue);
    }
    else
    {
        op2 = temp_table[node2->it];
        temp_top--;
    }
    // ret+= + "\tmov ebx, " + temp_table[node2->it] + "\n"
    // ��ʵ??����������ȫ���ü��أ���Ϊ֧��???������ַ����???
    ret += "\t";
    if (op == "-")
    {
        //??����??��???һ�׼Ĵ���, ����??����??��Ҫ֧�ּӼ���???
        ret += "sub eax, " + op2 + "\n";
    }
    else if (op == "+")
    {
        ret += "add eax, " + op2 + "\n";
    }
    else if (op == "*")
    {
        ret += "imul eax, " + op2 + "\n";
    }
    else if (op == "/")
    {
        // �������������edx?
        ret += "mov edx, 0\n\t";
        ret += "mov ebx, " + op2 + "\n";
        ret += "idiv ebx\n";
    }
    else if (op == "%")
    {
        // �����Ľ��������edx???
        ret += "mov edx, 0\n\t";
        ret += "mov ebx, " + op2 + "\n";
        ret += "idiv ebx\n";
    }
    else if (op == "^")
    {
        ret += "xor eax, " + op2 + "\n";
    }
    else if (op == "|")
    {
        ret += "or eax, " + op2 + "\n";
    }
    else if (op == "&")
    {
        ret += "and eax, " + op2 + "\n";
    }
    // �����ڵķ���ֵ��ͬ��????Ҫ�޸������
    // ��Ҫ����ڵ�ĵ�ַ
    else if (op == "=")
    {
        // �õ�node1���ڷ��ű��д洢�ı�����
        ret += "mov ebx, " + op2;
        ret += "mov " + op1 + ", ebx\n";
    }
    else if (op == ">>=")
    {
        ret += "\tmov ecx, " + op2;
        ret += "sar eax, cl\n";
        ret += "mov " + op1 + ", eax\n";
    }
    else if (op == "<<=")
    {
        ret += "\tmov ecx, " + op2;
        ret += "sal eax, cl\n";
        ret += "mov " + op1 + ", eax\n";
    }
    else if (op == "+=")
    {
        ret += "add eax, " + op2 + "\n";
        ret += "mov " + op1 + ", eax\n";
    }
    else if (op == "-=")
    {
        ret += "sub eax, " + op2 + "\n";
        ret += "mov " + op1 + ", eax\n";
    }
    else if (op == "*=")
    {
        ret += "imul eax, " + op2 + "\n";
        ret += "mov " + op1 + ", eax\n";
    }
    else if (op == "/=")
    {
        ret += "mov edx, 0\n\t";
        ret += "mov ebx, " + op2 + "\n";
        ret += "idiv ebx\n";
        ret += "mov " + op1 + ", eax\n";
    }
    else if (op == "%=")
    {
        ret += "mov edx, 0\n\t";
        ret += "mov ebx, " + op2 + "\n";
        ret += "idiv ebx\n";
        ret += "mov " + op1 + ", edx\n";
    }
    else if (op == "&=")
    {
        ret += "and eax, " + op2 + "\n";
        ret += "mov " + op1 + ", eax\n";
    }
    else if (op == "^=")
    {
        ret += "xor eax, " + op2 + "\n";
        ret += "mov " + op1 + ", eax\n";
    }
    else if (op == "|=")
    {
        ret += "or eax, " + op2 + "\n";
        ret += "mov " + op1 + ", eax\n";
    }
    else if (op == ">>")
    {
        ret += "\tmov ecx, " + op2;
        ret += "sar eax, cl\n";
    }
    else if (op == "<<")
    {
        ret += "\tmov ecx, " + op2;
        ret += "sal eax, cl\n";
    }
    temp_top++;
    if (temp_top > max_top)
    {
        max_top++;
        temp_table[temp_top] = "temp_" + to_string(temp_top);
    }
    // ȡģ������ṹ��edx�У�������eax��
    if (op == "%")
        ret += "\tmov " + temp_table[temp_top] + ", edx\n";
    else
        ret += "\tmov " + temp_table[temp_top] + ", eax\n";
    return ret;
}

string generate_double_code(Node *node1, Node *node2, string op)
{
    // ����??������??
    string ret;
    string op1; // op1 �������Ǳ�ʶ���ͳ�����û����ʱ����
    if (node1->it == -1)
    {
        if (node1->nd_type == ID_t)
            op1 = node1->name;
        else
            op1 = to_string(node1->value.fvalue);
    }
    else
    {
        op1 = temp_table[node1->it];
        temp_top--;
    }
    ret += "\t mov eax, " + op1 + "\n";
    string op2;
    if (node2->it == -1)
    {
        op2 = to_string(node2->value.fvalue);
    }
    else
    {
        op2 = temp_table[node2->it];
        temp_top--;
    }
    if (op == "+")
    {
        ret += "\tfld " + op2 + "\n\t";
        ret += "fadd\n";
    }
    else if (op == "-")
    {
        ret += "\tfld " + op2 + "\n\t";
        ret += "fsub\n";
    }
    else if (op == "*")
    {
        ret += "\tfld " + op2 + "\n\t";
        ret += "fmul\n";
    }
    else if (op == "/")
    {
        ret += "\tfld " + op2 + "\n\t";
        ret += "fdiv\n";
    }
    else if (op == "=")
    {
        // �Ȱ�node1����, ��ѹ??node2����node2��ֵ����node1
        ret += "fstp " + node1->name + "\n";
        ret += "\tfld " + op2 + "\n\t";
        ret += "fstp " + node2->name + "\n";
    }
    else if (op == "+=")
    {
        ret += "\tfld " + op2 + "\n\t";
        ret += "fadd\n";
        // �������Ĵ洢
        ret += "fst " + node1->name + "\n";
    }
    else if (op == "-=")
    {
        ret += "\tfld " + op2 + "\n\t";
        ret += "fsub\n";
        ret += "fst " + node1->name + "\n";
    }
    else if (op == "*=")
    {
        ret += "\tfld " + op2 + "\n\t";
        ret += "fmul\n";
        ret += "fst " + node1->name + "\n";
    }
    else if (op == "/=")
    {
        ret += "\tfld " + op2 + "\n\t";
        ret += "fdiv\n";
        ret += "fst " + node1->name + "\n";
    }
    temp_top++;
    if (temp_top > max_top)
    {
        max_top++;
        temp_table[temp_top] = "temp_" + to_string(temp_top);
    }
    ret += "\t fstp" + temp_table[temp_top] + "\n";
    return ret;
}

// ǰ׺��Ŀ���???
string generate_pre_code(Node *node, string op)
{
    // ����һ������һ��Ӧ���Ǳ�ʶ���������Ǳ��ʽ
    string ret;
    string op1; // op1 �������Ǳ�ʶ���ͳ�����û����ʱ����
    if (node->it == -1)
    {
        if (node->nd_type == ID_t)
            op1 = node->name;
        else
        {
            cout << "���ɱ���ֵ����ֵ: at line: " << line << endl;
        }
    }
    else
    {
        op1 = temp_table[node->it];
        temp_top--;
    }
    ret += "\t mov eax, " + op1 + "\n";
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
        ret += "\tnot eax\n";
    }
    else if (op == "!")
    {
        // �߼�???
        //�Ȳ�ʵ��
    }
    else if (op == "++")
    {
        ret += "\tinc eax\n";
        ret += "\tmov " + node->name + ", eax\n";
    }
    else if (op == "--")
    {
        ret += "\tdec eax\n";
        ret += "\tmov " + node->name + ", eax\n";
    }
    temp_top++;
    if (temp_top > max_top)
    {
        max_top++;
        temp_table[max_top] = "temp_" + to_string(max_top);
    }
    ret += "\tmov " + temp_table[temp_top] + ", eax\n";
    return ret;
}
// ��׺��Ŀ���???
string generate_post_code(Node *node, string op)
{
    string ret;
    string op1; // op1 �������Ǳ�ʶ���ͳ�����û����ʱ����
    if (node->it == -1)
    {
        if (node->nd_type == ID_t)
            op1 = node->name;
        else
        {
            cout << "���ɱ���ֵ����ֵ: at line: " << line << endl;
        }
    }
    else
    {
        op1 = temp_table[node->it];
        temp_top--;
    }
    ret += "\t mov eax, " + op1 + "\n";

    if (op == "++")
    {
        ret += "\tinc eax\n";
    }
    else if (op == "--")
    {
        ret += "\tdec eax]\n";
    }
    // ��׺���ʽ����֮ǰ��ֵ������
    temp_top++;
    if (temp_top > max_top)
    {
        max_top++;
        temp_table[max_top] = "temp_" + to_string(max_top);
    }
    ret += "\tmov " + temp_table[temp_top] + ", eax\n";
    return ret;
}

string generate_var_define()
{
    string ret = ".DATA\n";
    auto i = Var_Table.begin();
    for (; i != Var_Table.end(); i++)
    {
        VarEntry entry = i->second;
        ret += "\t" + entry.name + " ";
        if (entry.type == Double)
            ret += "real8 ";
        else
        {
            ret += "dd ";
        }
        if (entry.state = Valid)
        {
            switch (entry.type)
            {
            case Double:
                ret += to_string(entry.value.fvalue) + "\n";
                break;
            default:
                ret += to_string(entry.value.ivalue) + "\n";
                break;
            }
        }
        else
        {
            ret += "?\n";
        }
    }
    for (int i = 0; i < max_top; i++)
    {
        // ��ӡ��ʱ������
        ret += "\t" + temp_table[i] + "\tdd ?\n";
    }
    return ret;
}

string generate_header()
{
    string ret;
    ret+= "\t.586\n";
    ret+= "\t.model flat, stdcall\n";
    ret+= "\toption casemap :none\n";
    ret+= "\n";
    ret+= "\tinclude \\masm32\\include\\windows.inc\n";
    ret+= "\tinclude \\masm32\\include\\user32.inc\n";
    ret+= "\tinclude \\masm32\\include\\kernel32.inc\n";
    ret+= "\tinclude \\masm32\\include\\masm32.inc\n";
    ret+= "\n";
    ret+= "\tincludelib \\masm32\\lib\\user32.lib\n";
    ret+= "\tincludelib \\masm32\\lib\\kernel32.lib\n";
    ret+= "\tincludelib \\masm32\\lib\\masm32.lib\n";
    return ret;
}