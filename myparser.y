%{
/****************************************************************************
myparser.y
ParserWizard generated YACC file.

****************************************************************************/

#include "mylexer.h"
#include<iostream>
#include<vector>
#include<string>
using namespace std;

string temp_operator;
extern int line;
int temp_top = 0;
int max_top = -1;
vector<string> temp_table; 
%}

/////////////////////////////////////////////////////////////////////////////
// declarations section

// parser name
%name myparser

// class definition
{
	// place any extra class members here
}

// constructor
{
	// place any extra initialisation code here
}

// destructor
{
	// place any extra cleanup code here
}

// attribute type
%include {
#ifndef YYSTYPE
#define YYSTYPE Node*
#endif
#include "tree.h"
}

// place any declarations here
%token IDENTIFIER CONSTANT STRING_LITERAL
%token INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token CHAR INT DOUBLE VOID
%token BOOL 
%token STRUCT UNION

%token IF ELSE WHILE DO FOR CONTINUE BREAK RETURN

%start translation_unit

%%
primary_expression
	: IDENTIFIER {
		// ���ͼ��
		if($1->state == Not_Def){
			cout<<"δ�����ʶ�� "<<$1->name<<" at line "<<line<<endl;
		}
		$$ = $1;
	}
	| CONSTANT	{$$ = $1;}
	| STRING_LITERAL {$$ = $1;}
	| '(' expression ')' {$$ = $2;}
	;

postfix_expression
	: primary_expression	{$$ = $1;}
	| postfix_expression '[' expression ']'	{cout<<"[]";}
	| postfix_expression '(' ')'	{cout<<"()";}
	| postfix_expression '(' argument_expression_list ')'	{cout<<"arguement list"<<endl;}
	| postfix_expression '.' IDENTIFIER	{
		// �ṹ������ȡ���ýṹ�ĳ�Ա
		cout<<"."<<endl;
	}
	| postfix_expression INC_OP {
		$$ = generate_expr_node();
		$$->children[0] = $1;
		$$->code = $1->code + generate_post_code($1, "++");
		$$->it = temp_top;
		$$->v_type = $1->v_type;
	}
	| postfix_expression DEC_OP {
		$$ = generate_expr_node();
		$$->children[0] = $1;
		$$->code = $1->code + generate_post_code($1, "--");
		$$->it = temp_top;
		$$->v_type = $1->v_type;
	}
	;

argument_expression_list
	: assignment_expression {

	}
	| argument_expression_list ',' assignment_expression {

	}
	;

unary_expression
	: postfix_expression {$$ = $1;}
	| INC_OP unary_expression {
		$$ = generate_expr_node();
		$$->children[0] = $2;
		$$->code = $2->code + generate_post_code($2, "++");
		$$->it = temp_top;
		$$->v_type = $2->v_type;
	}
	| DEC_OP unary_expression {
		$$ = generate_expr_node();
		$$->children[0] = $2;
		$$->code = $2->code + generate_post_code($2, "--");
		$$->it = temp_top;
		$$->v_type = $2->v_type;
	}
	| unary_operator unary_expression {
		$$ = generate_expr_node();
		$$->children[0] = $2;
		$$->code = $2->code + generate_pre_code($2, temp_operator);
		$$->it = temp_top;
		$$->v_type = $2->v_type;
	}
	;

unary_operator
	: '&' {temp_operator = "&";}
	| '*' {temp_operator = "*";}
	| '-' {temp_operator = "-";}
	| '~' {temp_operator = "~";}
	| '!' {temp_operator = "!";}
	;

multiplicative_expression
	: unary_expression {$$ = $1;}
	| multiplicative_expression '*' unary_expression {
		$$ = generate_expr_node();
		$$->children[0] = $1;
		$$->children[1] = $3;
		$1->sibing = $3;
		$$->code = $1->code + $3->code; // �ȰѺ��ӵĴ������
		if($1->v_type != $3->v_type)
		{
			$$->state = Type_Err;
			cout<<"error in line "<<line<<endl;
		}
		else
			$$->v_type = $1->v_type;
		if($1->v_type == Double)
			$$->code += generate_double_code($1,$3,"*");
		else
			$$->code += generate_expr_code($1,$3,"*");
		$$->it = temp_top;
		// ��Լ��������ʱ��������Ҫ����һ��
		
	}
	| multiplicative_expression '/' unary_expression {
		$$ = generate_expr_node();
		$$->children[0] = $1;
		$$->children[1] = $3;
		$1->sibing = $3;

		$$->code = $1->code + $3->code; // �ȰѺ��ӵĴ������
		if($1->v_type != $3->v_type)
		{
			$$->state = Type_Err;
			cout<<"error in line "<<line<<endl;
		}
		else
			$$->v_type = $1->v_type;
		if($1->v_type == Double)
			$$->code += generate_double_code($1,$3,"/");
		else
			$$->code += generate_expr_code($1,$3,"/");
		$$->it = temp_top;
		// ��Լ��������ʱ��������Ҫ����һ��
		
	}
	| multiplicative_expression '%' unary_expression {
		$$ = generate_expr_node();
		$$->children[0] = $1;
		$$->children[1] = $3;
		$1->sibing = $3;

		$$->code = $1->code + $3->code; // �ȰѺ��ӵĴ������
		if($1->v_type != $3->v_type)
		{
			$$->state = Type_Err;
			cout<<"error in line "<<line<<endl;
		}
		else
			$$->v_type = $1->v_type;
		
		$$->code += generate_expr_code($1,$3,"%");
		$$->it = temp_top;
		// ��Լ��������ʱ��������Ҫ����һ��
		
	}
	;

additive_expression
	: multiplicative_expression {$$ = $1;}
	| additive_expression '+' multiplicative_expression	{
		$$ = generate_expr_node();
		$$->children[0] = $1;
		$$->children[1] = $3;
		$1->sibing = $3;
		// �������+�Ÿ����﷨�Ƶ����룬��������ߵľ���
		$$->code = $1->code + $3->code; // �ȰѺ��ӵĴ������
		if($1->v_type != $3->v_type)
		{
			$$->state = Type_Err;
			cout<<"error in line "<<line<<endl;
		}
		else
			$$->v_type = $1->v_type;
		if($1->v_type == Double)
			$$->code += generate_double_code($1,$3,"+");
		else
			$$->code += generate_expr_code($1,$3,"+");
		$$->it = temp_top;
		// ��Լ��������ʱ��������Ҫ����һ��
		
	}
	| additive_expression '-' multiplicative_expression {
		$$->code = $1->code + $3->code; // �ȰѺ��ӵĴ������
		if($1->v_type != $3->v_type)
		{
			$$->state = Type_Err;
			cout<<"error in line "<<line<<endl;
		}
		else
			$$->v_type = $1->v_type;
		if($1->v_type == Double)
			$$->code += generate_double_code($1,$3,"-");
		else
			$$->code += generate_expr_code($1,$3,"-");
		$$->it = temp_top;
		// ��Լ��������ʱ��������Ҫ����һ��
		
	}
	;

shift_expression
	: additive_expression	{$$ = $1;}
	| shift_expression LEFT_OP additive_expression {
		$$ = generate_expr_node();
		$$->children[0] = $1;
		$$->children[1] = $3;
		$1->sibing = $3;

		$$->code = $1->code + $3->code; // �ȰѺ��ӵĴ������
		if($1->v_type != $3->v_type)
		{
			$$->state = Type_Err;
			cout<<"error in line "<<line<<endl;
		}
		else
			$$->v_type = $1->v_type;
		$$->code += generate_expr_code($1,$3,"<<");
		$$->it = temp_top;
		// ��Լ��������ʱ��������Ҫ����һ��
		

	}
	| shift_expression RIGHT_OP additive_expression {
		$$ = generate_expr_node();
		$$->children[0] = $1;
		$$->children[1] = $3;
		$1->sibing = $3;

		$$->code = $1->code + $3->code; // �ȰѺ��ӵĴ������
		if($1->v_type != $3->v_type)
		{
			$$->state = Type_Err;
			cout<<"error in line "<<line<<endl;
		}
		else
			$$->v_type = $1->v_type;
		$$->code += generate_expr_code($1,$3,">>");
		$$->it = temp_top;
		// ��Լ��������ʱ��������Ҫ����һ��
		
	}
	;

relational_expression
	: shift_expression	{$$ = $1;}
	| relational_expression '<' shift_expression {
		$$ = generate_expr_node();
		$$->children[0] = $1;
		$$->children[1] = $3;
		$1->sibing = $3;
	}
	| relational_expression '>' shift_expression {
		$$ = generate_expr_node();
		$$->children[0] = $1;
		$$->children[1] = $3;
		$1->sibing = $3;
	}
	| relational_expression LE_OP shift_expression {
		$$ = generate_expr_node();
		$$->children[0] = $1;
		$$->children[1] = $3;
		$1->sibing = $3;
	}
	| relational_expression GE_OP shift_expression {
		$$ = generate_expr_node();
		$$->children[0] = $1;
		$$->children[1] = $3;
		$1->sibing = $3;
	}
	;

equality_expression
	: relational_expression {$$ = $1;}
	| equality_expression EQ_OP relational_expression {
		$$ = generate_expr_node();
		$$->children[0] = $1;
		$$->children[1] = $3;
		$1->sibing = $3;
	}
	| equality_expression NE_OP relational_expression {
		$$ = generate_expr_node();
		$$->children[0] = $1;
		$$->children[1] = $3;
		$1->sibing = $3;
	}
	;

and_expression
	: equality_expression {$$ = $1;}
	| and_expression '&' equality_expression {
		$$ = generate_expr_node();
		$$->children[0] = $1;
		$$->children[1] = $3;
		$1->sibing = $3;

		$$->code = $1->code + $3->code; // �ȰѺ��ӵĴ������
		if($1->v_type != $3->v_type)
		{
			$$->state = Type_Err;
			cout<<"error in line "<<line<<endl;
		}
		else
			$$->v_type = $1->v_type;
		$$->code += generate_expr_code($1,$3,"&");
		$$->it = temp_top;
		// ��Լ��������ʱ��������Ҫ����һ��
		
	}
	;

exclusive_or_expression
	: and_expression {$$ = $1;}
	| exclusive_or_expression '^' and_expression {
		$$ = generate_expr_node();
		$$->children[0] = $1;
		$$->children[1] = $3;
		$1->sibing = $3;

		$$->code = $1->code + $3->code; // �ȰѺ��ӵĴ������
		if($1->v_type != $3->v_type)
		{
			$$->state = Type_Err;
			cout<<"error in line "<<line<<endl;
		}
		else
			$$->v_type = $1->v_type;
		$$->code += generate_expr_code($1,$3,"^");
		$$->it = temp_top;
		// ��Լ��������ʱ��������Ҫ����һ��
		
	}
	;

inclusive_or_expression
	: exclusive_or_expression {$$ = $1;}
	| inclusive_or_expression '|' exclusive_or_expression {
		$$ = generate_expr_node();
		$$->children[0] = $1;
		$$->children[1] = $3;
		$1->sibing = $3;

		$$->code = $1->code + $3->code; // �ȰѺ��ӵĴ������
		if($1->v_type != $3->v_type)
		{
			$$->state = Type_Err;
			cout<<"error in line "<<line<<endl;
		}
		else
			$$->v_type = $1->v_type;
		$$->code += generate_expr_code($1,$3,"|");
		$$->it = temp_top;
		// ��Լ��������ʱ��������Ҫ����һ��
		
	}
	;

logical_and_expression
	: inclusive_or_expression {$$ = $1;}
	| logical_and_expression AND_OP inclusive_or_expression {
		$$ = generate_expr_node();
		$$->children[0] = $1;
		$$->children[1] = $3;
		$1->sibing = $3;
	}
	;

logical_or_expression
	: logical_and_expression {$$ = $1;}
	| logical_or_expression OR_OP logical_and_expression {
		$$ = generate_expr_node();
		$$->children[0] = $1;
		$$->children[1] = $3;
		$1->sibing = $3;
	}
	;


assignment_expression
	: logical_or_expression {$$ = $1;}
	| unary_expression assignment_operator assignment_expression {
		// �����������ֱ�Ӿ���id������ *i= ������ &i = 
		// �Լ� a[i] = 
		// ֱ�Ӿ���id
		// if($1->name == ""){
		// 	Node*temp = $1;
		// 	while(temp->children[0] != NULL){
		// 		temp = temp->children[0];
		// 	}
		// }
		$$ = generate_expr_node();
		$$->children[0] = $1;
		$$->children[1] = $3;
		$1->sibing = $3;

		$$->code = $1->code + $3->code; // �ȰѺ��ӵĴ������
		if($1->v_type != $3->v_type)
		{
			$$->state = Type_Err;
			cout<<"error in line "<<line<<endl;
		}
		else
			$$->v_type = $1->v_type;
		$$->code = $1->code + $3->code; // �ȰѺ��ӵĴ������
		if($3->v_type == Double)
			$$->code += generate_double_code($1, $3, temp_operator);
		else
			$$->code += generate_expr_code($1, $3, temp_operator);
		$$->it = temp_top;
		// ��Լ��������ʱ��������Ҫ����һ��
		
	}
	;

assignment_operator
	: '=' {temp_operator = "=";}
	| MUL_ASSIGN {temp_operator = "*=";}
	| DIV_ASSIGN {temp_operator = "/=";}
	| MOD_ASSIGN {temp_operator = "%=";}
	| ADD_ASSIGN {temp_operator = "+=";}
	| SUB_ASSIGN {temp_operator = "-=";}
	| LEFT_ASSIGN {temp_operator = "<<=";}
	| RIGHT_ASSIGN {temp_operator = ">>=";}
	| AND_ASSIGN {temp_operator = "&=";}
	| XOR_ASSIGN {temp_operator = "^=";}
	| OR_ASSIGN {temp_operator = "|=";}
	;

expression
	: assignment_expression {$$ = $1;}
	| expression ',' assignment_expression {
		//
	}
	;

constant_expression
	: logical_or_expression
	;

declaration
	: declaration_specifiers init_declarator_list ';' {
		// �����������
		// ����init_declarator_list, �������������ű�
		$$ = generate_decl_node();
		Node*temp = $2;
		while(temp != NULL){
			ID_Table[temp->name] = "VAR";
			VarEntry entry;
			entry.name = $1->name;
			entry.type = $1->v_type;
			Value value;
			if(temp->has_value){
				switch(entry.type){
					case Integer:
						value.ivalue = temp->value.ivalue;
						break;
					case Double:
						value.fvalue = temp->value.fvalue;
						break;
					case Char:
						value.cvalue =  temp->value.cvalue;
						break;
					case Boolean:
					 	value.ivalue = temp->value.ivalue;
						break;
					default:
						cout<<"error at line: "<<line<<endl;
				}
				entry.value = value;
			}
			Var_Table[temp->name] = entry;
			// ��Ҫ�����ʼ���Ĵ������
			$$->code += temp->code;
			temp = temp->sibing;
		}
		// ����������Щ��Ҫ���ӳ�ʼ���Ĵ�����
	}
	;

declaration_specifiers
	: type_specifier {$$ = $1;}
	| type_specifier declaration_specifiers
	;

init_declarator_list
	: init_declarator {$$ = $1; }
	| init_declarator_list ',' init_declarator
	;

init_declarator
	: declarator {
		// a
		$$ = $1;
		// ������ű�״̬Ϊδ��ʼ��
		$$->state = Not_Init;
	}
	| declarator '=' initializer {
		// a=1
		// ����ó�����ʼ������$3->has_value=true
		if($3->has_value){
			$$ = $1;
			$$->has_value = true;
			$$->value = $3->value;
			$$->svalue = $3->svalue;
			$$->state = Valid;
		}
		else{
			// �������ɸ�ֵ���Ĵ���
			$$->code = $3->code; // �ȰѼ������Ĵ������
			if($3->v_type == Double)
				$$->code += generate_double_code($1,$3,"-");
			else
				$$->code += generate_expr_code($1,$3,"-");
			$$->it = temp_top;
			// ��Լ��������ʱ��������Ҫ����һ��
			
		}

	}
	;


type_specifier
	: VOID {}
	| CHAR {$$ = new Node();$$->v_type = Char;}
	| INT {$$ = new Node();$$->v_type = Integer;}
	| DOUBLE {$$ = new Node();$$->v_type = Double;}
	| BOOL {$$ = new Node();$$->v_type = Boolean;}
	| struct_or_union_specifier
	| TYPE_NAME
	;

struct_or_union_specifier
	: struct_or_union IDENTIFIER '{' struct_declaration_list '}'	{
		//cout<<"struct!"<<endl;
	}
	| struct_or_union '{' struct_declaration_list '}'
	| struct_or_union IDENTIFIER
	;

struct_or_union
	: STRUCT
	| UNION
	;

struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';'
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list
	| type_specifier
	;

struct_declarator_list
	: struct_declarator
	| struct_declarator_list ',' struct_declarator
	;

struct_declarator
	: declarator
	| ':' constant_expression
	| declarator ':' constant_expression
	;



declarator
	: pointer direct_declarator {
		// declarator *a

	}
	| direct_declarator {
		// a
		$$ = $1;
	}
	;


direct_declarator
	: IDENTIFIER {
		$$ = $1;
		cout<< "��������ʶ��"<<endl;
	}
	| '(' declarator ')'
	| direct_declarator '[' assignment_expression ']'
	| direct_declarator '[' '*' ']'
	| direct_declarator '[' ']'
	| direct_declarator '(' parameter_type_list ')'
	| direct_declarator '(' identifier_list ')'
	| direct_declarator '(' ')'
	;

pointer
	: '*'
	| '*' pointer
	;

parameter_type_list
	: parameter_list
	;

parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration
	;

parameter_declaration
	: declaration_specifiers declarator
	| declaration_specifiers abstract_declarator
	| declaration_specifiers
	;

identifier_list
	: IDENTIFIER
	| identifier_list ',' IDENTIFIER
	;


abstract_declarator
	: pointer
	| direct_abstract_declarator
	| pointer direct_abstract_declarator
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'
	| '[' ']'
	| '[' assignment_expression ']'
	| direct_abstract_declarator '[' ']'
	| direct_abstract_declarator '[' assignment_expression ']'
	| '[' '*' ']'
	| direct_abstract_declarator '[' '*' ']'
	| '(' ')'
	| '(' parameter_type_list ')'
	| direct_abstract_declarator '(' ')'
	| direct_abstract_declarator '(' parameter_type_list ')'
	;

initializer
	: assignment_expression {
		// �㰡�����ñ��ʽ����ʼ�����ǳ�ʼ��ֵ����û�е���
		$$ = $1;
	}
	| '{' initializer_list '}'
	| '{' initializer_list ',' '}'
	;

initializer_list
	: initializer
	| designation initializer
	| initializer_list ',' initializer
	| initializer_list ',' designation initializer
	;

designation
	: designator_list '='
	;

designator_list
	: designator
	| designator_list designator
	;

designator
	: '[' constant_expression ']'	{printf("designator");}
	| '.' IDENTIFIER
	;

statement
	: compound_statement { $$ = $1;}
	| expression_statement {$$ = $1;}
	| selection_statement
	| iteration_statement
	| jump_statement
	;



compound_statement
	: '{' '}'
	| '{' block_item_list '}' {
		$$ = generate_stmt_node();
		Node*temp = $2;
		while(temp != NULL){
			$$->code += temp->code;
			temp = temp->sibing;
		}
	}
	;

block_item_list
	: block_item {$$ = $1;}
	| block_item_list block_item {
		// Ҫ������
		$$ = $1;
		$$->sibing = $2;
	}
	;

block_item
	: declaration {$$ = $1;}
	| statement {$$ = $1;}
	;

expression_statement
	: ';'
	| expression ';' {
		$$ = generate_stmt_node();
		$$->code = $1->code;
	}
	;

selection_statement
	: IF '(' expression ')' statement
	| IF '(' expression ')' statement ELSE statement
	;

iteration_statement
	: WHILE '(' expression ')' statement
	| DO statement WHILE '(' expression ')' ';'
	| FOR '(' expression_statement expression_statement ')' statement
	| FOR '(' expression_statement expression_statement expression ')' statement
	| FOR '(' declaration expression_statement ')' statement
	| FOR '(' declaration expression_statement expression ')' statement
	;

jump_statement
	: CONTINUE ';'
	| BREAK ';'
	| RETURN ';'
	| RETURN expression ';'
	;

translation_unit
	: external_declaration {cout<<$1->code;}
	| translation_unit external_declaration 
	;

external_declaration
	: function_definition {}
	| declaration
	;

function_definition
	: declaration_specifiers declarator declaration_list compound_statement {

	}
	| declaration_specifiers declarator compound_statement {
		// �޲κ�������
		$$ = generate_stmt_node();
		$$->code += $2->name + " proc\n";
		$$->code += $3->code;
		cout<<$$->code;
	}			
	;

declaration_list
	: declaration {

	}
	| declaration_list declaration {

	}
	;

/////////////////////////////////////////////////////////////////////////////
// rules section

// place your YACC rules here (there must be at least one)


%%
#include <stdio.h>
#include<fstream>
using namespace std;
extern char yytext[];
extern int column;

void yyerror(char const *s)
{
	fflush(stdout);
	printf("\n%*s\n%*s\n", column, "^", column, s);
}
/////////////////////////////////////////////////////////////////////////////
// programs section

int main(int argc, char*argv[])
{
	int n = 1;
	mylexer lexer;
	myparser parser;
	if (parser.yycreate(&lexer)) {
		if (lexer.yycreate(&parser)) {
			// �����Գɹ����Ժ��ٸ��ļ�
			// lexer.yyin = new ifstream(argv[1]);
			// lexer.yyout = new ofstream(argv[2]);
			n = parser.yyparse();
			cout<<generate_header();
			cout<<generate_var_define();
			// parse_tree.get_label();
			// parse_tree.gen_code(*lexer.yyout);
		}
	}
	system("pause");
	return n;
}

