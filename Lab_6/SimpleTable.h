#ifndef __STREAM_TABLE_H
#define __STREAM_TABLE_H

#undef max
#undef min
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstring>
//��������� ������������ ������ �������
#define CRLF "\n"

//��������� ���������� �������� �������
//#define CRLF std::endl

/**
* ������������� ������� � ������������� ����� � ��������
* ��������� ��� � ������� C++
*/
class StreamTable {
public:
    std::ostream& os_;
    StreamTable(std::ostream& os = std::cout, char delimRow = ' ', char delimCol = ' ') :
        os_(os),
        borderExtOn_(true),
        delimRowOn_(true),
        delimRow_(delimRow),
        delimColOn_(true),
        delimCol_(delimCol),
        colIndex_(0),
        firstCell_(1) {}

    virtual ~StreamTable() {}

    virtual std::ostream& os() const {
        return os_;
    }

    //���������� ������� �������?
    void MakeBorderExt(bool on) {
        borderExtOn_ = on;
    }

    //������ ����������� �����
    void SetDelimRow(bool delimOn, char delimRow = ' ') {
        delimRowOn_ = delimOn;
        if (delimRowOn_)
            delimRow_ = delimRow;
        else if (!delimColOn_)
            MakeBorderExt(false);
    }

    //������ ����������� ��������
    void SetDelimCol(bool delimOn, char delimCol = ' ') {
        delimColOn_ = delimOn;
        if (delimColOn_)
            delimCol_ = delimCol;
        else if (!delimRowOn_)
            MakeBorderExt(false);
    }

    int AddCol(int colWidth, bool visible = true) {
        colWidth_.push_back(colWidth);
        visible_.push_back(visible);
        return colWidth_.back();
    }

    void SetVisible(int col, bool flg) {
        visible_[col - 1] = flg;
    }

    void SetCols(int colCount, int colWidth = 0) {
        Clear();

        for (int ic = 0; ic < colCount; ic++) {
            AddCol(colWidth);
        }
    }

    virtual void Clear() {
        colWidth_.clear();
        visible_.clear();
        colIndex_ = 0;
        firstCell_ = 1;
    }

    void AddEmptyRow() {
        for (int ic = 0; ic < (int)colWidth_.size(); ic++) {
            *this << "";
        }
    }

    template <typename T> StreamTable& operator << (const T& obj) {
        Push(obj);
        return *this;
    }

    StreamTable& operator << (const std::string& s) {
        colWidth_[colIndex_] = std::max(colWidth_[colIndex_], (int)s.size() + 1);
        Push(s);
        return *this;
    }

    StreamTable& operator << (const char* s) {
        colWidth_[colIndex_] = std::max(colWidth_[colIndex_], (int)strlen(s) + 1);
        Push(s);
        return *this;
    }

private:
    bool borderExtOn_;
    bool delimRowOn_;
    char delimRow_;


    bool delimColOn_;
    char delimCol_;

    int colIndex_;

    std::vector<int> colWidth_;
    bool firstCell_;
    std::vector<int> visible_;

    template <typename T>
    void Push(const T& obj) {
        if (firstCell_) {
            if (borderExtOn_)
                MakeRowBorder();

            firstCell_ = 0;
        }

        if (visible_[colIndex_]) {
            DelimCol();

            os_.width(colWidth_[colIndex_]);
            os_.fill(' ');
            os_ << obj;
        }

        if (++colIndex_ == (int)colWidth_.size()) {
            DelimCol();

            if (delimRowOn_)
                MakeRowBorder();
            else
                os_ << CRLF;

            colIndex_ = 0;
        }
    }

    void MakeRowBorder() {
        os_ << CRLF;
        DelimCol();

        int ic;
        for (ic = 0; ic < (int)colWidth_.size(); ic++) {
            if (visible_[ic]) {
                os_.width(colWidth_[ic] + 1);
                os_.fill(delimRow_);
                DelimCol();
            }
        }
        os_ << CRLF;
    }

    void DelimCol() {
        if (delimColOn_ && (borderExtOn_ || colIndex_))
            os_ << delimCol_;
        else
            os_ << ' ';
    }


    //������ �� �����������
    StreamTable& operator = (const StreamTable&);
};

#endif // __STREAM_TABLE_H