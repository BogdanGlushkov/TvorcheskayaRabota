#include "widget.h"
#include "ui_widget.h"
#include <QtMath>
#include <cmath>
#include <QFile>
#include <QTextStream>
#include <boost/filesystem/operations.hpp>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

#include <QButtonGroup>
#include <QPushButton>


short int originalRadix = 10;
char hexDigits[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

string hexDigitsInDecimal[16] = {
    "0", "1", "2", "3", "4", "5", "6", "7", "8",
    "9", "10", "11", "12", "13", "14", "15"};

QString leftOperand = "";
QString rightOperand = "";
QString tempString = "";
char Operator = 'c';
bool isOperationOn = false;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    this->setLayout(layout);
    QMenuBar *menubar = new QMenuBar(this);
    layout->setMenuBar(menubar);
    QMenu *menu = new QMenu("Выбрать тему", this);
    QAction *darkTheme = new QAction("Темная", this);
    menu->addAction(darkTheme);
    QAction *defaultTheme = new QAction("Светлая", this);
    menu->addAction(defaultTheme);
    QAction *blueTheme = new QAction("Синяя", this);
    menu->addAction(blueTheme);
    menubar->addMenu(menu);
    ui->radioBtn_10Notation->setChecked(true);
    ui->textEdit->setAlignment(Qt::AlignRight);

    QMenu *menu2 = new QMenu("Выбрать режим работы", this);
    QAction *numeralSystemsConversion = new QAction("Перевод из одной СС в другую", this);
    menu2->addAction(numeralSystemsConversion);
    QAction *calculator = new QAction("Калькулятор СС", this);
    menu2->addAction(calculator);
    menubar->addMenu(menu2);

    on_default_theme();
    connect(darkTheme, SIGNAL(triggered()), this, SLOT(on_dark_theme()));
    connect(blueTheme, SIGNAL(triggered()), this, SLOT(on_blue_theme()));
    connect(defaultTheme, SIGNAL(triggered()), this, SLOT(on_default_theme()));

    ConversionOnlyModeOn();
    connect(numeralSystemsConversion, SIGNAL(triggered()), this, SLOT(ConversionOnlyModeOn()));
    connect(calculator, SIGNAL(triggered()), this, SLOT(CalculatorModeOn()));

}

void Widget::on_default_theme(){
    QFile styleFile(":/QtCSS/style.css");
    styleFile.open( QFile::ReadOnly );
    QString style( styleFile.readAll( ) );
    qApp->setStyleSheet(style);
    styleFile.close();
}

void Widget::on_dark_theme(){
    QFile styleFile(":/style_dark.css");
    styleFile.open( QFile::ReadOnly );
    QString style( styleFile.readAll( ) );
    qApp->setStyleSheet(style);
    styleFile.close( );
}

void Widget::on_blue_theme(){
    QFile styleFile(":/style_blue.css");
    styleFile.open( QFile::ReadOnly );
    QString style( styleFile.readAll( ) );
    qApp->setStyleSheet(style);
    styleFile.close( );
}

Widget::~Widget()
{
    delete ui;
}

void Widget::SplitOnIntegerAndFractionalParts(string number, string &integer_part, string &fractional_part){
    integer_part = "";
    fractional_part = "";
    int index = 0;
    while (number[index] != ',' && index != number.length()){
        integer_part += number[index];
        index++;
    }
    index++;
    while (index < number.length()){
        fractional_part += number[index];
        index++;
    }
}

bool Widget::HasComma(string str){
    for (int i = 0; i < str.length(); i++)
        if (str[i] == ',')
            return true;
    return false;
}

void Widget::DeleteZeroAtTheEnd(string &str){
    short count_zero = 0;
    string new_str = "";
    if (HasComma(str)) {
        for (int i = str.length()-1; i >= 0; i--)
            if (str[i] == '0')
                count_zero++;
            else
                break;
        for (int i = 0; i < str.length()-count_zero; i++)
            new_str += str[i];
        str = new_str;
    }
}

string Widget::HornersScheme(short orig_radix){
    string originalNumber = ui->textEdit->toPlainText().toLocal8Bit().constData();
    string originalNumber_int_part = "", originalNumber_fractional_part = "";
    string decNumberConvertedToString = "";
    SplitOnIntegerAndFractionalParts(originalNumber, originalNumber_int_part, originalNumber_fractional_part);
    cpp_int decNumber_int_part(0);
    long double decNumber_fract_part = 0;
    unsigned short digit;
    //работа с целой частью
    for (int i = 0; i < originalNumber_int_part.size()-1; i++){
        digit = 0;
        while (originalNumber_int_part[i] != hexDigits[digit])
            digit++;
        decNumber_int_part += digit;
        decNumber_int_part *= orig_radix;
    }
    digit = 0;
    while (originalNumber_int_part[originalNumber_int_part.size()-1] != hexDigits[digit])
        digit++;
    decNumber_int_part += digit;
    //работа с дробной частью
    for (int i = originalNumber_fractional_part.size()-1; i >= 0; i--){
        digit = 0;
        while (originalNumber_fractional_part[i] != hexDigits[digit])
            digit++;
        decNumber_fract_part += digit;
        decNumber_fract_part /= orig_radix;
    }
    string temp = to_string(decNumber_fract_part);
    string fract_part = "";
    if (decNumber_fract_part > 0) {
        for (int i = 2; i < temp.length(); i++)
            fract_part += temp[i];
        fract_part = ',' + fract_part;
    }
    decNumberConvertedToString = boost::lexical_cast<string>(decNumber_int_part) + fract_part;
    DeleteZeroAtTheEnd(decNumberConvertedToString);
    return decNumberConvertedToString;
}

string Widget::ConvertFromDecToAnyNumberSystem(string dec_number, short final_radix){
    string decNumber_int_part_STRING = "", decNumber_fract_part_STRING = "";
    string convertedNumber = "";
    SplitOnIntegerAndFractionalParts(dec_number, decNumber_int_part_STRING, decNumber_fract_part_STRING);
    cpp_int decNumber_int_part_cppint(decNumber_int_part_STRING);
    int digit;
    string digit_converted_to_string = "";
    //перевод целой части
    while (decNumber_int_part_cppint > 0){
        digit_converted_to_string = boost::lexical_cast<string>(decNumber_int_part_cppint % final_radix);
        digit = 0;
        while (digit_converted_to_string != hexDigitsInDecimal[digit])
            digit++;
        convertedNumber = hexDigits[digit] + convertedNumber;
        decNumber_int_part_cppint /= final_radix;
    }
    //перевод дробной части
    decNumber_fract_part_STRING = "0." + decNumber_fract_part_STRING;
    long double decNumber_fraction_LONGDOUBLE = stold(decNumber_fract_part_STRING, nullptr);
    if (convertedNumber == "")
        convertedNumber = "0";
    if (decNumber_fraction_LONGDOUBLE != 0.0)
        convertedNumber += ',';
    int index = 0;
    while (decNumber_fraction_LONGDOUBLE != 0 && index < 20){
        decNumber_fraction_LONGDOUBLE *= final_radix;
        digit_converted_to_string = to_string((int)(decNumber_fraction_LONGDOUBLE));
        digit = 0;
        while (digit_converted_to_string != hexDigitsInDecimal[digit])
            digit++;
        convertedNumber += hexDigits[digit];
        decNumber_fraction_LONGDOUBLE = decNumber_fraction_LONGDOUBLE - std::trunc(decNumber_fraction_LONGDOUBLE);
        index++;
    }
    DeleteZeroAtTheEnd(convertedNumber);
    return convertedNumber;
}

string Widget::ConvertFromOneNumberSystemToAnother(short orig_radix, short final_radix){
    if (final_radix == 10)
        return HornersScheme(orig_radix);
    else {
        if (orig_radix == 10)
            return ConvertFromDecToAnyNumberSystem(ui->textEdit->toPlainText().toLocal8Bit().constData(), final_radix);
        else
            return ConvertFromDecToAnyNumberSystem(HornersScheme(orig_radix), final_radix);
    }
}

void Widget::SetNumberAtTextEdit(short final_radix){
    if (!ui->textEdit->toPlainText().isEmpty()){
        ui->textEdit->setText(QString::fromStdString(ConvertFromOneNumberSystemToAnother(originalRadix, final_radix)));
        ui->textEdit->setAlignment(Qt::AlignRight);
    }
}

void Widget::on_radioBtn_2Notation_clicked()
{
   SetNumberAtTextEdit(2);
   originalRadix = 2;
}

void Widget::on_radioBtn_3Notation_clicked()
{
    SetNumberAtTextEdit(3);
    originalRadix = 3;
}

void Widget::on_radioBtn_8Notation_clicked()
{
    SetNumberAtTextEdit(8);
    originalRadix = 8;
}

void Widget::on_radioBtn_9Notation_clicked()
{
    SetNumberAtTextEdit(9);
    originalRadix = 9;
}

void Widget::on_radioBtn_10Notation_clicked()
{
   SetNumberAtTextEdit(10);
   originalRadix = 10;
}

void Widget::on_radioBtn_16Notation_clicked()
{
    SetNumberAtTextEdit(16);
    originalRadix = 16;
}

void Widget::on_pushButton_digit_0_clicked()
{
    if (!isOperationOn) {
        leftOperand = ui->textEdit->toPlainText();
        leftOperand += '0';
        ui->textEdit->setText(leftOperand);
        ui->textEdit->setAlignment(Qt::AlignRight);
    } else {
        tempString = ui->textEdit->toPlainText();
        rightOperand += '0';
        tempString += '0';
        ui->textEdit->setText(tempString);
        ui->textEdit->setAlignment(Qt::AlignRight);
    }
}

void Widget::on_pushButton_digit_1_clicked()
{
    if (!isOperationOn) {
        leftOperand = ui->textEdit->toPlainText();
        leftOperand += '1';
        ui->textEdit->setText(leftOperand);
        ui->textEdit->setAlignment(Qt::AlignRight);
    } else {
        tempString = ui->textEdit->toPlainText();
        rightOperand += '1';
        tempString += '1';
        ui->textEdit->setText(tempString);
        ui->textEdit->setAlignment(Qt::AlignRight);
    }
}

void Widget::on_pushButton_digit_2_clicked()
{
    if (!isOperationOn) {
        leftOperand = ui->textEdit->toPlainText();
        leftOperand += '2';
        ui->textEdit->setText(leftOperand);
        ui->textEdit->setAlignment(Qt::AlignRight);
    } else {
        tempString = ui->textEdit->toPlainText();
        rightOperand += '2';
        tempString += '2';
        ui->textEdit->setText(tempString);
        ui->textEdit->setAlignment(Qt::AlignRight);
    }
}

void Widget::on_pushButton_digit_3_clicked()
{   if (!isOperationOn) {
        leftOperand = ui->textEdit->toPlainText();
        leftOperand += '3';
        ui->textEdit->setText(leftOperand);
        ui->textEdit->setAlignment(Qt::AlignRight);
    } else {
        tempString = ui->textEdit->toPlainText();
        rightOperand += '3';
        tempString += '3';
        ui->textEdit->setText(tempString);
        ui->textEdit->setAlignment(Qt::AlignRight);
    }
}

void Widget::on_pushButton_digit_4_clicked()
{
    if (!isOperationOn) {
        leftOperand = ui->textEdit->toPlainText();
        leftOperand += '4';
        ui->textEdit->setText(leftOperand);
        ui->textEdit->setAlignment(Qt::AlignRight);
    } else {
        tempString = ui->textEdit->toPlainText();
        rightOperand += '4';
        tempString += '4';
        ui->textEdit->setText(tempString);
        ui->textEdit->setAlignment(Qt::AlignRight);
    }
}

void Widget::on_pushButton_digit_5_clicked()
{
    if (!isOperationOn) {
        leftOperand = ui->textEdit->toPlainText();
        leftOperand += '5';
        ui->textEdit->setText(leftOperand);
        ui->textEdit->setAlignment(Qt::AlignRight);
    } else {
        tempString = ui->textEdit->toPlainText();
        rightOperand += '5';
        tempString += '5';
        ui->textEdit->setText(tempString);
        ui->textEdit->setAlignment(Qt::AlignRight);
    }
}

void Widget::on_pushButton_digit_6_clicked()
{
    if (!isOperationOn) {
        leftOperand = ui->textEdit->toPlainText();
        leftOperand += '6';
        ui->textEdit->setText(leftOperand);
        ui->textEdit->setAlignment(Qt::AlignRight);
    } else {
        tempString = ui->textEdit->toPlainText();
        rightOperand += '6';
        tempString += '6';
        ui->textEdit->setText(tempString);
        ui->textEdit->setAlignment(Qt::AlignRight);
    }
}

void Widget::on_pushButton_digit_7_clicked()
{
    if (!isOperationOn) {
        leftOperand = ui->textEdit->toPlainText();
        leftOperand += '7';
        ui->textEdit->setText(leftOperand);
        ui->textEdit->setAlignment(Qt::AlignRight);
    } else {
        tempString = ui->textEdit->toPlainText();
        rightOperand += '7';
        tempString += '7';
        ui->textEdit->setText(tempString);
        ui->textEdit->setAlignment(Qt::AlignRight);
    }
}

void Widget::on_pushButton_digit_8_clicked()
{
    if (!isOperationOn) {
        leftOperand = ui->textEdit->toPlainText();
        leftOperand += '8';
        ui->textEdit->setText(leftOperand);
        ui->textEdit->setAlignment(Qt::AlignRight);
    } else {
        tempString = ui->textEdit->toPlainText();
        rightOperand += '8';
        tempString += '8';
        ui->textEdit->setText(tempString);
        ui->textEdit->setAlignment(Qt::AlignRight);
    }
}

void Widget::on_pushButton_digit_9_clicked()
{
    if (!isOperationOn) {
        leftOperand = ui->textEdit->toPlainText();
        leftOperand += '9';
        ui->textEdit->setText(leftOperand);
        ui->textEdit->setAlignment(Qt::AlignRight);
    } else {
        tempString = ui->textEdit->toPlainText();
        rightOperand += '9';
        tempString += '9';
        ui->textEdit->setText(tempString);
        ui->textEdit->setAlignment(Qt::AlignRight);
    }
}

void Widget::on_pushButton_comma_clicked()
{
    if (!isOperationOn) {
        leftOperand = ui->textEdit->toPlainText();
        leftOperand += ',';
        ui->textEdit->setText(leftOperand);
        ui->textEdit->setAlignment(Qt::AlignRight);
    } else {
        tempString = ui->textEdit->toPlainText();
        rightOperand += ',';
        tempString += ',';
        ui->textEdit->setText(tempString);
        ui->textEdit->setAlignment(Qt::AlignRight);
    }
}

void Widget::on_pushButton_operator_plus_clicked()
{
    tempString = ui->textEdit->toPlainText();
    Operator = '+';
    tempString += Operator;
    ui->textEdit->setText(tempString);
    ui->textEdit->setAlignment(Qt::AlignRight);
    isOperationOn = true;
}

void Widget::on_pushButton_operator_minus_clicked()
{
    tempString = ui->textEdit->toPlainText();
    Operator = '-';
    tempString += Operator;
    ui->textEdit->setText(tempString);
    ui->textEdit->setAlignment(Qt::AlignRight);
    isOperationOn = true;
}

void Widget::on_pushButton_operator_multiplication_clicked()
{
    tempString = ui->textEdit->toPlainText();
    Operator = '*';
    tempString += Operator;
    ui->textEdit->setText(tempString);
    ui->textEdit->setAlignment(Qt::AlignRight);
    isOperationOn = true;
}

void Widget::on_pushButton_operator_division_clicked()
{
    tempString = ui->textEdit->toPlainText();
    Operator = '/';
    tempString += Operator;
    ui->textEdit->setText(tempString);
    ui->textEdit->setAlignment(Qt::AlignRight);
    isOperationOn = true;
}

void Widget::on_pushButton_clear_field_clicked()
{
    leftOperand = ui->textEdit->toPlainText();
    leftOperand = "";
    rightOperand = "";
    Operator = 'c';
    isOperationOn = false;
    ui->textEdit->setText(leftOperand);
    ui->textEdit->setAlignment(Qt::AlignRight);
}

void Widget::on_pushButton_operator_equals_clicked()
{
    bool isCommaInLeftOperand = HasComma(leftOperand.toLocal8Bit().constData());
    bool isCommaInRightOperand = HasComma(rightOperand.toLocal8Bit().constData());

    cpp_dec_float_100 result_DECFLOAT(0);
    cpp_int result_CPPINT(0);
    if (isCommaInLeftOperand || isCommaInRightOperand) {
        cpp_dec_float_100 leftOperand_DECFLOAT(leftOperand.replace(',', '.').toLocal8Bit().constData());
        cpp_dec_float_100 rightOperand_DECFLOAT(rightOperand.replace(',', '.').toLocal8Bit().constData());
        switch (Operator){
        case '+':
            result_DECFLOAT = leftOperand_DECFLOAT + rightOperand_DECFLOAT;
            break;
        case '-':
            result_DECFLOAT = leftOperand_DECFLOAT - rightOperand_DECFLOAT;
            break;
        case '*':
            result_DECFLOAT = leftOperand_DECFLOAT * rightOperand_DECFLOAT;
            break;
        case '/':
            result_DECFLOAT = leftOperand_DECFLOAT / rightOperand_DECFLOAT;
            break;
        }

    } else {
        cpp_int leftOperand_CPPINT(leftOperand.toLocal8Bit().constData());
        cpp_int rightOperand_CPPINT(rightOperand.toLocal8Bit().constData());
        result_CPPINT = leftOperand_CPPINT + rightOperand_CPPINT;
        switch (Operator){
        case '+':
            result_CPPINT = leftOperand_CPPINT + rightOperand_CPPINT;
            break;
        case '-':
            result_CPPINT = leftOperand_CPPINT - rightOperand_CPPINT;
            break;
        case '*':
            result_CPPINT = leftOperand_CPPINT * rightOperand_CPPINT;
            break;
        case '/':
            cpp_dec_float_100 left(leftOperand.toLocal8Bit().constData());
            cpp_dec_float_100 right(rightOperand.toLocal8Bit().constData());
            result_DECFLOAT = left / right;
            break;
        }
    }


    QString numberConvertedToQString = "";
    switch (Operator){
    case '+':
        if (isCommaInLeftOperand || isCommaInRightOperand){
            numberConvertedToQString = QString::fromStdString(boost::lexical_cast<string>(result_DECFLOAT));
            numberConvertedToQString.replace('.', ',');
            ui->textEdit->setText(numberConvertedToQString);
        } else {
            numberConvertedToQString = QString::fromStdString(boost::lexical_cast<string>(result_CPPINT));
            ui->textEdit->setText(numberConvertedToQString);
        }
        ui->textEdit->setAlignment(Qt::AlignRight);
        isOperationOn = false;
        break;
    case '-':
        if (isCommaInLeftOperand || isCommaInRightOperand){
            numberConvertedToQString = QString::fromStdString(boost::lexical_cast<string>(result_DECFLOAT));
            numberConvertedToQString.replace('.', ',');
            ui->textEdit->setText(numberConvertedToQString);
        } else {
            numberConvertedToQString = QString::fromStdString(boost::lexical_cast<string>(result_CPPINT));
            ui->textEdit->setText(numberConvertedToQString);
        }
        ui->textEdit->setAlignment(Qt::AlignRight);
        isOperationOn = false;
        break;
    case '*':
        if (isCommaInLeftOperand || isCommaInRightOperand){
            numberConvertedToQString = QString::fromStdString(boost::lexical_cast<string>(result_DECFLOAT));
            numberConvertedToQString.replace('.', ',');
            ui->textEdit->setText(numberConvertedToQString);
        } else {
            numberConvertedToQString = QString::fromStdString(boost::lexical_cast<string>(result_CPPINT));
            ui->textEdit->setText(numberConvertedToQString);
        }
        ui->textEdit->setAlignment(Qt::AlignRight);
        isOperationOn = false;
        break;
    case '/':
        numberConvertedToQString = QString::fromStdString(boost::lexical_cast<string>(result_DECFLOAT));
        numberConvertedToQString.replace('.', ',');
        ui->textEdit->setText(numberConvertedToQString);
        ui->textEdit->setAlignment(Qt::AlignRight);
        isOperationOn = false;
        break;
    }
    leftOperand = ui->textEdit->toPlainText();
    rightOperand = "";
}

void Widget::ShowButtons(){
    ui->pushButton_digit_0->setVisible(true);
    ui->pushButton_digit_1->setVisible(true);
    ui->pushButton_digit_2->setVisible(true);
    ui->pushButton_digit_3->setVisible(true);
    ui->pushButton_digit_4->setVisible(true);
    ui->pushButton_digit_5->setVisible(true);
    ui->pushButton_digit_6->setVisible(true);
    ui->pushButton_digit_7->setVisible(true);
    ui->pushButton_digit_8->setVisible(true);
    ui->pushButton_digit_9->setVisible(true);
    ui->pushButton_comma->setVisible(true);
    ui->pushButton_clear_field->setVisible(true);
    ui->pushButton_operator_plus->setVisible(true);
    ui->pushButton_operator_minus->setVisible(true);
    ui->pushButton_operator_multiplication->setVisible(true);
    ui->pushButton_operator_division->setVisible(true);
    ui->pushButton_operator_equals->setVisible(true);
}

void Widget::HideButtons(){
    ui->pushButton_digit_0->setVisible(false);
    ui->pushButton_digit_1->setVisible(false);
    ui->pushButton_digit_2->setVisible(false);
    ui->pushButton_digit_3->setVisible(false);
    ui->pushButton_digit_4->setVisible(false);
    ui->pushButton_digit_5->setVisible(false);
    ui->pushButton_digit_6->setVisible(false);
    ui->pushButton_digit_7->setVisible(false);
    ui->pushButton_digit_8->setVisible(false);
    ui->pushButton_digit_9->setVisible(false);
    ui->pushButton_comma->setVisible(false);
    ui->pushButton_clear_field->setVisible(false);
    ui->pushButton_operator_plus->setVisible(false);
    ui->pushButton_operator_minus->setVisible(false);
    ui->pushButton_operator_multiplication->setVisible(false);
    ui->pushButton_operator_division->setVisible(false);
    ui->pushButton_operator_equals->setVisible(false);
}

void Widget::CalculatorModeOn(){
    this->setFixedHeight(580);
    ShowButtons();
}

void Widget::ConversionOnlyModeOn(){
    this->setFixedHeight(350);
    HideButtons();
}
