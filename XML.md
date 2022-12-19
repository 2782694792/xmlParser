# XML

    可扩展标记语言（e**X**tensible **M**arkup **L**anguage）。

- 被设计用来传输和存储数据，不用于表现和展示数据，HTML 则用来表现数据；

- **独立于软件和硬件的信息传输工具**；

- 主要用到的有可扩展标记语言、可扩展样式语言（XSL）、XBRL和XPath等；

- 没有预定义，需要自定义标签。

```xml
<?xml version="1.0" encoding="UTF-8"?> <!-- 声明 -->
<site> <!-- 开始标签 -->
  <name>RUNOOB</name>
  <url>https://www.runoob.com</url>
  <logo>runoob-logo.png</logo>
  <desc>编程学习网站</desc>
</site> <!-- 结束标签 -->
```

## 用途

- 通过读取 xml 更新网页数据内容，把数据从 HTML 分离；

- 以纯文本格式进行存储，简化数据共享；

- 较好的兼容性，简化了数据传输和平台变更；

## 树结构

```xml
<?xml version="1.0" encoding="UTF-8"?>
<note> <!-- 根元素 -->
    <to>Tove</to> <!-- 子元素 -->
    <from>Jani</from>
    <heading>Reminder</heading>
    <body>Don't forget me this weekend!</body>
</note>
```

- 必须包含**根元素**。该元素是所有其他元素的父元素；

- 所有的元素都可以有文本内容和属性。

## 语法

### 声明

```xml
<?xml version="1.0" encoding="utf-8"?>
```

### 标签

- 所有的 XML 元素都必须有一个关闭标签

```xml
<p>This is a paragraph.</p>
<br />
```

- 对大小写敏感；

- 属性值必须加引号；

```xml
<note date="12/11/2007">
```

- 特殊字符：

> 以 LF 存储换行:
> 
> Windows : 回车符（CR）和换行符（LF）。

- 预定义实体引用：

| &lt ;   | <   | less than      |
| ------- | --- | -------------- |
| &gt ;   | >   | greater than   |
| &amp ;  | &   | ampersand      |
| &apos ; | '   | apostrophe     |
| &quot ; | "   | quotation mark |

## 元素

从（且包括）开始标签直到（且包括）结束标签的部分。

### 命名规则

- 可以包含字母、数字以及其他的字符；
- 不能以数字或者标点符号开始；
- 不能以字母 xml（或者 XML、Xml 等等）开始；
- 不能包含空格。

### 可扩展

```xml
<note>
    <date>2008-01-10</date>
    <to>Tove</to>
    <from>Jani</from>
    <heading>Reminder</heading>
    <body>Don't forget me this weekend!</body>
</note>
```

## 属性

    属性（Attribute）提供有关元素的额外信息。

- 必须加引号；

```xml
<person sex="female">
<person sex='female'>
<gangster name='George "Shotgun" Ziegler'>
<gangster name="George "Shotgun" Ziegler">
```

- 不能包含多个值、树结构，不容易扩展；

- 分配 ID 索引可用于标识 XML 元素。

## 验证

```xml
<?xml version="1.0" encoding="ISO-8859-1"?>
<!DOCTYPE note SYSTEM "Note.dtd">
```

### XML DTD

定义 XML 文档的结构。它使用一系列合法的元素来定义文档结构：

```xml
<!DOCTYPE note
[
<!ELEMENT note (to,from,heading,body)>
<!ELEMENT to (#PCDATA)>
<!ELEMENT from (#PCDATA)>
<!ELEMENT heading (#PCDATA)>
<!ELEMENT body (#PCDATA)>
]>
```

### XML Schema

W3C 支持一种基于 XML 的 DTD 代替者，为 XML Schema：

```xml
<xs:element name="note">

<xs:complexType>
<xs:sequence>
<xs:element name="to" type="xs:string"/>
<xs:element name="from" type="xs:string"/>
<xs:element name="heading" type="xs:string"/>
<xs:element name="body" type="xs:string"/>
</xs:sequence>
</xs:complexType>

</xs:element>
```

# XML 解析

    实现简单的 XML 文件解析，通常是要在可读 xml 文件的解读上进行，既要对文件内容解析并序列化，同时能够在多层次解析上进行内容判断。

    首要目标就是对 xml 数据类型进行划分，竟然存在多层级的 xml 节点，那就需要进行递归解析，此时仅存在同级的节点，可利用 xml 类数组，其次是对 xml 节点的名称、属性、内容进行解析，中间还可能存在一系列的注释文本内容。

## XML 类与 Attribute 属性类

### 数据类型

```cpp
// 节点数组
list<Xml> *m_child;

// 节点名称
string *m_name;

// 节点属性，由属性名称和属性值组成
map<string, Attribute> *m_attrs;

// 节点内容，即节点的文本描述
string *m_text;
```

### 节点属性

    由于节点属性存在多种基本数据类型值，其中包括 bool、int、double、char 数组（string），

需要在基本数据类型的基础上作出总体封装，以类（**Attribute**）的形式将 int、bool、double、string 进行符号类型重载，同时对符号 `=` 进行类重载。

```cpp
operator bool();
operator int();
operator double();
operator string();

Attribute &operator=(bool attr);
Attribute &operator=(int attr);
Attribute &operator=(double attr);
Attribute &operator=(const char *attr);
Attribute &operator=(const string &attr);
Attribute &operator=(const Attribute &attr);
```

### 处理数据类型数据

    明确了数据类型的定义，解析时需要对对应的数据类型进行设置（存储）和获取（解析），进行如下定义：

```cpp
string getName() const; // 获取节点名称
void setName(const string &name); // 设置节点名称

string getText() const; // 获取节点文本内容
void setText(const string &text); // 设置节点文本内容

string getAttr(const string &key) const; // 获取节点名称 key 所对应的属性值
void setAttr(const string &key, const Attribute &value); // 设置节点属性名称和值
```

### 节点数组

    明确了节点数据类型，在完成解析时，判断节点的层次则需要对节点数组大小 `size` 进行判定，若是对节点数组进行扩展，则需要进行节点数组符号 `[]` 进行重载，同时包括子节点的操作：插入 `append`、删除 `remove`：

```cpp
// 子节点的数量
int size() const;

// 插入子节点 child
void append(const Xml &child);

// 添加指定节点
Xml &operator[](int index); // 不存在对应索引，赋值添加子节点
Xml &operator[](const char *name);
Xml &operator[](const string &name); // 不存在对应节点名称，添加子节点

// 删除对应索引、节点名称的数组节点
void remove(int index);
void remove(const char *name);
void remove(const string &name);
```

### 数组节点遍历

    明确了节点数组的插入和删除，进行节点数组序列化输出的同时，包括对节点数组的指定位置进行遍历，通常有数组开头 **begin**、数组末尾 **end **，通过节点数组容器 `iterator` 的类型定义，完成更简洁的遍历操作：

```cpp
typedef std::list<Xml>::iterator iterator; // 节点数组容器
iterator begin(); // 节点数组开头
iterator end(); // 节点数组末尾
```

## Parser 解析类

    明确了 xml 类与 Attribute 属性类的组成和处理操作，接下来是对数据节点内容的整体解析，由于文本的整体性，需要进行节点细分，首先是进行数据字符判断，一个是文本，即字符串内容（包括文件流），另一个是索引，即字符索引：

```cpp
string m_str; // 字符串文本，xml 文本数据
int m_index; // 字符索引，检索当前字符

// 加载文件路径进行 xml 文件流 file 转换成字符串 m_str
bool loadFile(const string &file);
// 指定字符串内容 str 进行加载存入字符串文本 m_str
bool loadStr(const string &str);
```

### parse 解析

    明确了解析所需要的数据类型，接下来将对字符串文本 m_str 进行解析，通过解析 parse 函数进行对 xml 文件的声明 **declaration** 进行 `parse_declaration` 声明解析，对注释文本 **comment** 进行 `parse_comment` 注释解析，最后便是对节点 **element** 进行 `parse_element` 节点解析，其中包括对常用符号：空格、换行符、回车符、制表符进行 `skip_whitespace` 特殊处理：

```cpp
Xml parse(); // xml 解析

void skip_whitespace(); // 空格特殊处理
bool parse_declaration(); // 声明解析
bool parse_comment(); // 注释解析
Xml parse_element(); // 节点解析
```

### 节点解析

    对 xml 文件解析头部定义：声明、注释明确之后，便是对节点进行解析，其中就包括节点名称 `parse_element_Name` 解析，节点文本内容 `parse_element_Text` 解析，节点属性解析：节点属性名称 `parse_element_attrKey` 解析，节点属性值 `parse_element_attrValue` 解析，当然还包括对属性的文本内容进行解析，以及对节点注释、子节点进行再次解析：

```cpp
// 节点文本内容解析
string parse_element_Text();
// 节点名称解析
string parse_element_Name();
// 节点属性名称解析
string parse_element_attrKey();
// 节点属性值解析
string parse_element_attrValue();
```


