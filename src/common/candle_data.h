#include <vector>
#include <string>
#include "candle.h"
#include "errors.h"
#include "strutil.h"
#include "csv_parser.hpp"

class CandleData
{
public:

    void clear()
    {
        m_data.clear();
    }

    void load_csv(const std::string& file_name)
    {
        const char field_terminator = ',';
        const char line_terminator  = '\n';
        const char enclosure_char   = '"';

        csv_parser file_parser;

        /* Define how many records we're gonna skip. This could be used to skip the column definitions. */
        file_parser.set_skip_lines(0);

        /* Specify the file to parse */
        if (!file_parser.init(file_name.c_str()))
            THROW(FileError, "candle data: can't open csv file");

        /* Here we tell the parser how to parse the file */
        file_parser.set_enclosed_char(enclosure_char, ENCLOSURE_OPTIONAL);

        file_parser.set_field_term_char(field_terminator);

        file_parser.set_line_term_char(line_terminator);

        unsigned int line_count = 0U;

        int date_column, time_column, open_column, high_column, low_column;
        int close_column, volume_column;

        while(file_parser.has_more_rows())
        {
            csv_row row = file_parser.get_row();

            if (line_count == 0)
            {
                for (int i = 0; i < row.size(); i++)
                {
                    if      (row[i] == "date")
                        date_column = i;
                    else if (row[i] == "time")
                        time_column = i;
                    else if (row[i] == "open")
                        open_column = i;
                    else if (row[i] == "high")
                        high_column = i;
                    else if (row[i] == "low")
                        low_column = i;
                    else if (row[i] == "close")
                        close_column = i;
                    else if (row[i] == "volume")
                        volume_column = i;
                }
            }
            else
            {
                Candle cur_candle;

                for (int i = 0; i < row.size(); i++)
                {
                    if (i == date_column)
                    {
                        uint32_t date;
                        if (!StrUtil::from_str(row[i], &date))
                            THROW(ParseError, "Can't convert date at line: %d", line_count);

                        cur_candle.date = date;
                    }
                    else if (i == time_column)
                    {
                        uint32_t time;
                        if (!StrUtil::from_str(row[i], &time))
                            THROW(ParseError, "Can't convert timestamp at line: %d", line_count);

                        cur_candle.time = time;
                    }
                    else if (i == open_column)
                    {
                        double value;
                        if (!StrUtil::from_str(row[i], &value))
                            THROW(ParseError, "Can't convert open at line: %d", line_count);

                        cur_candle.open = value;
                    }
                    else if (i == high_column)
                    {
                        double value;
                        if (!StrUtil::from_str(row[i], &value))
                            THROW(ParseError, "Can't convert high at line: %d", line_count);

                        cur_candle.high = value;
                    }
                    else if (i == low_column)
                    {
                        double value;
                        if (!StrUtil::from_str(row[i], &value))
                            THROW(ParseError, "Can't convert low at line: %d", line_count);

                        cur_candle.low = value;
                    }
                    else if (i == close_column)
                    {
                        double value;
                        if (!StrUtil::from_str(row[i], &value))
                            THROW(ParseError, "Can't convert close at line: %d", line_count);

                        cur_candle.close = value;
                    }
                    else if (i == volume_column)
                    {
                        double value;
                        if (!StrUtil::from_str(row[i], &value))
                            THROW(ParseError, "Can't convert volume at line: %d", line_count);
                        cur_candle.volume = value;
                    }
                }

                m_data.push_back(cur_candle);
            }

            line_count++;
        }
    }


    const std::vector<Candle>& data() const    {      return m_data;    }

private:
    std::vector<Candle>  m_data;
};

