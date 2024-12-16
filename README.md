# Calculator.proj 实验指南

    The project of simulating a calculator for 24's cpl final work. 

## 前置知识
    
如果你不了解 **C 语言**中的  `struct`, `enum`, `union`, `typedef`  的用法，请你首先查阅课本或其他资料再来阅读本实验指南。
如果你的目标为 **100** 分，建议不要急于开始，首先阅读实验指南的全文以进行合理的顶层设计，这可能比在原来的基础上加东⻄更加节省时间。
    
## 总览
    
在读入了每一行语句之后，我们可能需要依次进行以下步骤。
1.  词法分析
2.  语法分析 & 表达式求值
3.  变量赋值
大家会在《编译原理》课程中学会真正的词法分析和语法分析。这里，我们大致描述一下这些步骤分别都是在做什么。

### 词法分析

当我们得到一个表达式，

``` C
1 + （ 33 + 2 ） * 4 - 5
```

我们要做的第一步就是将这个输入的字符串转化为计算机内部的数据表示，具体来说，我们需要按照分隔符（一个空格 ' ' ）将每个字符串分段，并求出每一段（称为 `token`）的类型。
比如，上面的表达式可以分成以下几个 `token` 。

``` C 
number     "1"
operator   "+"
operator   "("
number     "33"
...
```

你可以阅读 “语言规约” 部分来了解需要支持的不同 `token` 类型。
在词法分析结束之后，你可以将所有的 `token` 储存在一个连续的数组中，你可以使用一个  `tokens[]`  数组来连续储存所有 `token` 。

``` C
typedef struct token {
    int type;
    char str[32];
} Token;
Token tokens[...];
```

### 语法分析 & 表达式求值

#### 概述

这里的语法分析主要是分析表达式逻辑的正确性，比如  `1 + + 3`  和  `1 ( 9 )`  就不是逻辑上正确的表达式，而这个步骤通常可以和表达式求值一起完成。
当词法分析结束后，你应该得到所有的 `token` 的值，举个例子，

``` C
4 + 3 * （ 2 - 1 ） 
```

的 `token` 表达式为

``` C
+-----+-----+-----+-----+-----+-----+-----+-----+-----+ 
| NUM | '+' | NUM | '*' | '(' | NUM | '-' | NUM | ')' |
| "4" |     | "3" |     |     | "2" |     | "1" |     |
+-----+-----+-----+-----+-----+-----+-----+-----+-----+
```
#### 递归求值

根据表达式的归纳定义特性，我们可以很方便地使用递归来进行求值。
首先我们给出算数表达式的归纳定义：

``` C
<expr> ::=
    <integer>              # 一个数也是表达式
|   <variable>             # 一个变量也是表达式
|   "(" <expr> ")"         # 在表达式两边加个括号也是表达式
|   <expr> "+" <expr>      # 两个表达式相加也是表达式
|   <expr> "-" <expr>      # 接下来你全懂了
|   <expr> "*" <expr>
|   <expr> "/" <expr>
```

上面这种表示方法就是大名鼎鼎的 [BNF]( https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form ) , 任何一本正规的程序设计语言教程都会使用 `BNF` 来给出这种程序设计语言的语法.
为了在 token 表达式中指示一个子表达式，我们可以使用两个整数  `l`  和  `r`  来指示这个子表达式的开始位置和结束位置。这样我们就可以很容易把求值函数的框架写出来了:

``` C
eval(l, r) {
    if (l > r) {
        /* Wrong expression */
    }
    else if (l == r) {
        /* Single token.
         * For now this token should be a number or variable.
         * Return the value of the number or variable.
         */
    }
    else if (check_parentheses(l, r) == true) {
        /* The expression is surrounded by a matched pair of parentheses.
         * If that is the case, just throw away the parentheses.
         */
        return eval(l + 1, r - 1);
    }
    else {
        /* We should do more things here. */
    }
}
```

其中  `check_parentheses()`  函数用于判断
1.  表达式是否被一对匹配的括号包围着
2.  表达式的左右括号是否匹配
如果不匹配， 这个表达式肯定是不符合语法的，也就不需要继续进行求值了。
我们举一些例子来说明  `check_parentheses()` 函数的功能:

``` C
"( 2 - 1 )"              // true
"( 4 + 3 * ( 2 - 1 ) )"  // true
"4 + 3 * ( 2 - 1 )"      // false, the whole expression is not surrounded by a matched pair of parentheses
// pair of parentheses
"( ( 4 + 3 ) ) * ( ( 2 - 1 ) )" // false, bad expression
"( 4 + 3 ) * ( 2 - 1 )"       // false, the leftmost '(' and the rightmost ')' are not matched
```

上面的框架已经考虑了 `BNF` 中算术表达式的开头三种定义，接下来我们来考虑剩下的情况(即上述伪代码中最后一个 `else`  中的内容)。 一个问题是，给出一个最左边和最右边不同时是括号的⻓表达式，我们要怎么正确地将它分裂成两个子表达式?
我们定义"主运算符"为表达式人工求值时，最后一步进行运行的运算符, 它指示了表达式的类型(例如当一个表达式的最后一步是减法运算时, 它本质上是一个减法表达式).
要正确地对一个⻓表达式进行分裂，就是要找到它的主运算符。我们继续使用上面的例子来探讨这个问题：

``` C
"4 + 3 * ( 2 - 1 )"
/***************************/
case 1:
      "+"   
    /     \ 
"4"       "3 * ( 2 - 1 )"

case 2:

      "*" 
    /     \ 
"4 + 3"   "( 2 - 1 )"

case 3:
             "-"   
           /     \ 
"4 + 3 * ( 2"    "1 )"
```
 
上面列出了 `3` 种可能的分裂，注意到我们不可能在非运算符的 `token` 处进行分裂，否则分裂得到的结果均不是合法的表达式。
根据主运算符的定义，我们很容易发现，只有第一种分裂才是正确的。这其实也符合我们人工求值的过程：先算  `4`  和 `3 * ( 2 - 1 )` ，最后把它们的结果相加。第二种分裂违反了算术运算的优先级，它会导致加法比乘法更早进行。第三种分裂破坏了括号的平衡，分裂得到的结果均不是合法的表达式。
通过上面这个简单的例子，我们就可以总结出如何在一个 `token` 表达式中寻找主运算符了:
1.  非运算符的 `token` 不是主运算符.
2.  出现在一对括号中的 `token` 不是主运算符。注意到这里不会出现有括号包围整个表达式的情况，因为这种情况已经在  `check_parentheses()`  相应的 `if` 块中被处理了。
3.  主运算符的优先级在表达式中是最低的。这是因为主运算符是最后一步才进行的运算符。
当有多个运算符的优先级都是最低时，根据结合性，最后被结合的运算符才是主运算符。 一个例子是  `1 + 2 + 3` ，它的主运算符应该是右边的  `+` 。
要找出主运算符，只需要将  `tokens`  全部扫描一遍，就可以按照上述方法唯一确定主运算符。
找到了正确的主运算符之后，事情就变得很简单了：先对分裂出来的两个子表达式进行递归求值，然后再根据主运算符的类型对两个子表达式的值进行运算即可。于是完整的求值函数如下:

``` C
eval(l, r) {
    if (l > r) {
        /* Wrong expression */
    }
    else if (l == r) {
        /* Single token.
         * For now this token should be a number or variable.
         * Return the value of the number or variable.
         */
    }
    else if (check_parentheses(l, r) == 1) {
        /* The expression is surrounded by a matched pair of parentheses.
         * If that is the case, just throw away the parentheses.
         */
        return eval(l + 1, r - 1);
    }
    else {
        op = the position of 主运算符 in the token expression;
        val1 = eval(l, op - 1);
        val2 = eval(op + 1, r);

        switch (op_type) {

            case '+': return val1 + val2;
            case '-': /* ... */
            case '*': /* ... */
            case '/': /* ... */
            default: assert(0);
        }
    }
}
```

### 变量赋值

我们称变量赋值语句为 `assignment` ，那么
``` C
<assignment> ::
    <variable> "=" <expr>
``` 

首先，我们需要检查 `variable` 即变量名的合法性，如果不合法需要报错。
然后我们可以用一个结构体，记录下变量和对应的值，从而来记录现在有的所有的赋值，如（仅作建议，不强制）
``` C
typedef struct assignment {
    char name[32];
    int val;
} Assignment;
```

那么你就可以开一个 `Assignment` 类型的数组，把所有的赋值关系存下来。
当你在 `eval` 中遇到需要查询一个变量的值的时候，就只需要在这个数组里找到符合的，然后返回对应的值。
你也可以尝试使用不同的数据结构（比如哈希表）来提高查询的效率，但是在本项目中，数组（线性表）已经足够。

## 选做部分