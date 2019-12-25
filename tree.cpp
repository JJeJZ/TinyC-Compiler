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
    has_value = false;
    it = -1;
    ivalue = 0;
    fvalue = 0;

    begin_label = label_number;
    end_label = label_number;
    // next_label = -1;
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
            ret->name = result.name;
            ret->v_type = result.type;
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
void table_init()
{
    ID_Table["printf"] = "FUNC";
    ID_Table["scanf"] = "FUNC";
    FuncEntry f1, f2;
    f1.name = "printf";
    f1.type = None;
    f2.name = "scanf";
    f2.type = None;
    Fuction_Table["printf"] = f1;
    Fuction_Table["scanf"] = f2;
}
Node *generate_expr_node()
{
    Node *ret = NULL;
    ret = new Node();
    ret->nd_type = EXPR_t;
    // if (label_need)
    // {
    //     label_need--;
    //     ret->end_label = ret->begin_label = next_label;
    //     label_number++;
    //     ret->code = "L" + to_string(next_label) + ":\n";
    // }
    return ret;
}
Node *generate_stmt_node()
{
    Node *ret = NULL;
    ret = new Node();
    ret->nd_type = STMT_t;
    // if (label_need)
    // {
    //     label_need--;
    //     ret->end_label = ret->begin_label = next_label;
    //     label_number++;
    //     ret->code = "L" + to_string(next_label) + ":\n";
    // }

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
    ret->has_value = true;
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
            op1 = to_string(node1->ivalue);
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
        if (node2->nd_type == ID_t)
            op2 = node2->name;
        else
            op2 = to_string(node2->ivalue);
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
        ret += "\tidiv ebx\n";
    }
    else if (op == "%")
    {
        // �����Ľ��������edx???
        ret += "mov edx, 0\n\t";
        ret += "mov ebx, " + op2 + "\n";
        ret += "\tidiv ebx\n";
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
        ret = "";
        ret += "\tmov eax, " + op2 + "\n";
        ret += "\tmov " + op1 + ", eax\n";
    }
    else if (op == ">>=")
    {
        ret += "\tmov ecx, " + op2;
        ret += "\tsar eax, cl\n";
        ret += "\tmov " + op1 + ", eax\n";
    }
    else if (op == "<<=")
    {
        ret += "\tmov ecx, " + op2;
        ret += "\tsal eax, cl\n";
        ret += "\tmov " + op1 + ", eax\n";
    }
    else if (op == "+=")
    {
        ret += "add eax, " + op2 + "\n";
        ret += "\tmov " + op1 + ", eax\n";
    }
    else if (op == "-=")
    {
        ret += "sub eax, " + op2 + "\n";
        ret += "\tmov " + op1 + ", eax\n";
    }
    else if (op == "*=")
    {
        ret += "imul eax, " + op2 + "\n";
        ret += "\tmov " + op1 + ", eax\n";
    }
    else if (op == "/=")
    {
        ret += "mov edx, 0\n\t";
        ret += "mov ebx, " + op2 + "\n";
        ret += "\tidiv ebx\n";
        ret += "\tmov " + op1 + ", eax\n";
    }
    else if (op == "%=")
    {
        ret += "mov edx, 0\n\t";
        ret += "mov ebx, " + op2 + "\n";
        ret += "\tidiv ebx\n";
        ret += "\tmov " + op1 + ", edx\n";
    }
    else if (op == "&=")
    {
        ret += "and eax, " + op2 + "\n";
        ret += "\tmov " + op1 + ", eax\n";
    }
    else if (op == "^=")
    {
        ret += "xor eax, " + op2 + "\n";
        ret += "\tmov " + op1 + ", eax\n";
    }
    else if (op == "|=")
    {
        ret += "or eax, " + op2 + "\n";
        ret += "\tmov " + op1 + ", eax\n";
    }
    else if (op == ">>")
    {
        ret += "\tmov ecx, " + op2;
        ret += "\tsar eax, cl\n";
    }
    else if (op == "<<")
    {
        ret += "\tmov ecx, " + op2;
        ret += "\tsal eax, cl\n";
    }
    temp_top++;
    if (temp_top > max_top)
    {
        max_top++;
        temp_table.push_back("temp_" + to_string(max_top));
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
        {
            cout << "�������ǳ�ʼֵ�ĸ���������!" << endl;
        }
    }
    else
    {
        op1 = temp_table[node1->it];
        temp_top--;
    }
    ret += "\tfld " + op1 + "\n";
    string op2;
    if (node2->it == -1)
    {
        if (node2->nd_type == ID_t)
            op2 = node1->name;
        else
            cout << "�������ǳ�ʼֵ�ĸ���������!" << endl;
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
        ret += "\tfst " + node1->name + "\n";
    }
    else if (op == "-=")
    {
        ret += "\tfld " + op2 + "\n\t";
        ret += "fsub\n";
        ret += "\tfst " + node1->name + "\n";
    }
    else if (op == "*=")
    {
        ret += "\tfld " + op2 + "\n\t";
        ret += "fmul\n";
        ret += "\tfst " + node1->name + "\n";
    }
    else if (op == "/=")
    {
        ret += "\tfld " + op2 + "\n\t";
        ret += "fdiv\n";
        ret += "\tfst " + node1->name + "\n";
    }
    temp_top++;
    if (temp_top > max_top)
    {
        max_top++;
        temp_table.push_back("temp_" + to_string(max_top));
    }
    ret += "\tfstp " + temp_table[temp_top] + "\n";
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
    ret += "\tmov eax, " + op1 + "\n";
    if (op == "&")
    {
        // ȡ��ַ
        ret = "";
        ret += "\tmov eax, offset " + op1 + "\n";
    }
    else if (op == "*")
    {
        // ȡֵ
        ret = "";
        ret += "\tmov eax, [eax]\n";
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
        //���Լ����
        ret += "\tnot eax\n";
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
        temp_table.push_back("temp_" + to_string(max_top));
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
    ret += "\tmov eax, " + op1 + "\n";
    temp_top++;
    if (temp_top > max_top)
    {
        max_top++;
        temp_table.push_back("temp_" + to_string(max_top));
    }
    ret += "\tmov " + temp_table[temp_top] + ", eax\n";

    if (op == "++")
    {
        ret += "\tinc eax\n";
        ret += "\tmov " + node->name + ", eax\n";
    }
    else if (op == "--")
    {
        ret += "\tdec eax\n";
        ret += "\tmov " + node->name + ", eax\n";
    }
    // ��׺���ʽ����֮ǰ��ֵ������
    return ret;
}

string generate_bool_code(Node *node1, Node *node2, string op)
{
    string ret;
    // �ж����Ҳ�������id��������ʱ����
    string op1; // op1 �������Ǳ�ʶ���ͳ�����û����ʱ����
    if (node1->it == -1)
    {
        if (node1->nd_type == ID_t)
            op1 = node1->name;
        else
            op1 = to_string(node1->ivalue);
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
        if (node2->nd_type == ID_t)
            op2 = node2->name;
        else
            op2 = to_string(node2->ivalue);
    }
    else
    {
        op2 = temp_table[node2->it];
        temp_top--;
    }
    // ������������ɴ���
    if (op == "<=")
    {
        ret += "\tcmp eax, " + op2 + "\n";
        ret += "\tjle L" + to_string(label_number) + "\n";
    }
    else if (op == ">=")
    {
        ret += "\tcmp eax, " + op2 + "\n";
        ret += "\tjge L" + to_string(label_number) + "\n";
    }
    else if (op == "==")
    {
        ret += "\tcmp eax, " + op2 + "\n";
        ret += "\tje L" + to_string(label_number) + "\n";
    }
    else if (op == "!=")
    {
        ret += "\tcmp eax, " + op2 + "\n";
        ret += "\tjne L" + to_string(label_number) + "\n";
    }
    else if (op == "<")
    {
        ret += "\tcmp eax, " + op2 + "\n";
        ret += "\tjl L" + to_string(label_number) + "\n";
    }
    else if (op == ">")
    {
        ret += "\tcmp eax, " + op2 + "\n";
        ret += "\tjg L" + to_string(label_number) + "\n";
    }
    // ��������Ҫ��op1�ˣ��ټ�һ����ʱ����
    ret += "\tmov eax, 0\n";
    ret += "\tjmp L" + to_string(label_number + 1) + "\n";
    ret += "L" + to_string(label_number) + ":\n";
    ret += "\tmov eax, 1\n";
    label_number++;
    ret += "L" + to_string(label_number) + ":\n";
    label_number++;
    temp_top++;
    if (temp_top > max_top)
    {
        max_top++;
        temp_table.push_back("temp_" + to_string(max_top));
    }
    ret += "\tmov " + temp_table[temp_top] + ", eax\n";
    return ret;
}

string generate_and_or_code(Node *node1, Node *node2, string op)
{
    string ret;
    // �ж����Ҳ�������id��������ʱ����
    string op1; // op1 �������Ǳ�ʶ���ͳ�����û����ʱ����
    if (node1->it == -1)
    {
        if (node1->nd_type == ID_t)
            op1 = node1->name;
        else
            op1 = to_string(node1->ivalue);
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
        if (node2->nd_type == ID_t)
            op2 = node2->name;
        else
            op2 = to_string(node2->ivalue);
    }
    else
    {
        op2 = temp_table[node2->it];
        temp_top--;
    }
    if (op == "&&")
    {
        // ���ǰ�λ�벻һ����ʵ����Ĺ����� 010 �� 101
        ret += "\tand eax, " + op2 + "\n";
    }
    else if (op == "||")
    {
        // ��λ�����ʵ�ֻ�Ĺ���
        ret += "\tor eax, " + op2 + "\n";
    }
    temp_top++;
    if (temp_top > max_top)
    {
        max_top++;
        temp_table.push_back("temp_" + to_string(max_top));
    }
    ret += "\tmov " + temp_table[temp_top] + ", eax\n";
    return ret;
}
// expr stmt else stmt
string generate_if_code(Node *node1, Node *node2, Node *node3)
{
    string ret;
    ret += node1->code;
    string op1; // op1 �������Ǳ�ʶ���ͳ�����û����ʱ����
    if (node1->it == -1)
    {
        if (node1->nd_type == ID_t)
            op1 = node1->name;
        else
        {
            cout << "���ɱ���ֵ����ֵ: at line: " << line << endl;
        }
    }
    else
    {
        op1 = temp_table[node1->it];
        temp_top--;
    }
    // �Ƚ�
    ret += "\tcmp " + op1 + ", 0\n";
    // ���Ϊ0jmp�� false label
    ret += "\tje L" + to_string(label_number + 1) + "\n";
    ret += node2->code;
    // �����if else ��䣬ֱ�Ӿ���false label
    if (node3 != NULL)
    {
        label_number++;
        // �ղŵ������Ҫֱ����ת������
        ret += "\tjmp L" + to_string(label_number + 1) + "\n";
        ret += "L" + to_string(label_number) + ":\n";
        ret += node3->code;
    }
    // ����������һ�������
    // next_label��ռ���ˣ���Ӧ������ʹ��
    label_number++;
    ret += "L" + to_string(label_number) + ":\n";
    label_number++;
    return ret;
}

string generate_var_define()
{
    string ret = ".data\n";
    auto i = Var_Table.begin();
    for (; i != Var_Table.end(); i++)
    {
        VarEntry entry = i->second;
        ret += "\t" + entry.name + "\t\t";
        if (entry.type == Double)
            ret += "real8\t";
        else
        {
            ret += "dd\t\t";
        }
        if (entry.state == Valid)
        {
            switch (entry.type)
            {
            case Double:
                ret += to_string(entry.fvalue) + "\n";
                break;
            default:
                ret += to_string(entry.ivalue) + "\n";
                break;
            }
        }
        else
        {
            ret += "?\n";
        }
    }
    for (int i = 0; i <= max_top; i++)
    {
        // ��ӡ��ʱ������
        ret += "\t" + temp_table[i] + "\tdd\t\t?\n";
    }
    ret += "\n";
    // ��ӡ���뻺��
    ret += ".code\n";
    //    .code
    ret += "start:\n\tcall main\n\tinkey\n\tret\n\n";
    return ret;
}

string generate_header()
{
    string ret;
    ret += "\t.586\n";
    ret += "\t.model flat, stdcall\n";
    ret += "\toption casemap :none\n";
    ret += "\n";
    ret += "\tinclude \\masm32\\include\\windows.inc\n";
    ret += "\tinclude \\masm32\\include\\user32.inc\n";
    ret += "\tinclude \\masm32\\include\\kernel32.inc\n";
    ret += "\tinclude \\masm32\\include\\masm32.inc\n";
    ret += "\tinclude \\masm32\\macros\\macros.asm\n";
    ret += "\tinclude \\masm32\\include\\msvcrt.inc\n";
    ret += "\tinclude \\masm32\\include\\gdi32.inc\n";
    ret += "\n";
    ret += "\tincludelib \\masm32\\lib\\user32.lib\n";
    ret += "\tincludelib \\masm32\\lib\\kernel32.lib\n";
    ret += "\tincludelib \\masm32\\lib\\masm32.lib\n";
    ret += "\tincludelib \\masm32\\lib\\msvcrt.lib\n";
    ret += "\tincludelib \\masm32\\lib\\gdi32.lib\n\n";
    return ret;
}

void printVarTable()
{
    auto begin = Var_Table.begin();
    while (begin != Var_Table.end())
    {
        cout << begin->second.name << endl;
        begin++;
    }
}

void copyValue(Node *node1, Node *node2)
{
    node1->ivalue = node2->ivalue;
    node1->fvalue = node2->fvalue;
    node1->svalue = node2->svalue;
    node1->has_value = node2->has_value;
    node1->v_type = node2->v_type;
}
