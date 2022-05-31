#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <string>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>
using boost::multiprecision::cpp_int;
using boost::multiprecision::cpp_dec_float_100;
using namespace std;


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    string HornersScheme (short int orig_radix);
    string ConvertFromDecToAnyNumberSystem(string dec_number, short int final_radix);
    string ConvertFromOneNumberSystemToAnother(short int orig_radix, short int final_radix);
    void SetNumberAtTextEdit(short int final_radix);
    void SplitOnIntegerAndFractionalParts(string number, string& integer_part, string& fractional_part);
    void DeleteZeroAtTheEnd(string &str);
    bool HasComma(string str);
    void ShowButtons();
    void HideButtons();
private slots:


    void on_radioBtn_2Notation_clicked();

    void on_radioBtn_3Notation_clicked();

    void on_radioBtn_8Notation_clicked();

    void on_radioBtn_9Notation_clicked();

    void on_radioBtn_10Notation_clicked();

    void on_radioBtn_16Notation_clicked();

    void on_default_theme();

    void on_dark_theme();

    void on_blue_theme();

    void ConversionOnlyModeOn();

    void CalculatorModeOn();

    void on_pushButton_digit_0_clicked();

    void on_pushButton_digit_1_clicked();

    void on_pushButton_digit_2_clicked();

    void on_pushButton_digit_3_clicked();

    void on_pushButton_digit_4_clicked();

    void on_pushButton_digit_5_clicked();

    void on_pushButton_digit_6_clicked();

    void on_pushButton_digit_7_clicked();

    void on_pushButton_digit_8_clicked();

    void on_pushButton_digit_9_clicked();

    void on_pushButton_comma_clicked();

    void on_pushButton_operator_plus_clicked();

    void on_pushButton_operator_minus_clicked();

    void on_pushButton_operator_multiplication_clicked();

    void on_pushButton_operator_division_clicked();

    void on_pushButton_clear_field_clicked();

    void on_pushButton_operator_equals_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
