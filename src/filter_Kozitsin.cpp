#pragma once
#include "filters.hpp"

class Filter_Kozitsin : public Filters
{
public:
    virtual void boxFilter(const Matrix &src, Matrix& dst, const int kSize = 3)
    {
        if (src.cols() < kSize || src.rows() < kSize)
        {
            return;
        }
        else
        {
            dst.Zeros();
            // create filter
            // TODO: refactor -> move in new function
            Matrix filter(kSize, kSize);
            filter.Ones();
            // REFACTOR END

            int offset = floor(kSize / 2.0);

            Matrix emb(src.rows() + offset, src.cols() + offset);
            for (int i = offset; i < emb.rows() - offset; i++)
            {
                for (int j = offset; j < emb.cols() - offset; j++)
                {
                    emb.data()[i][j] = src.data()[i - offset][j - offset]; 
                }
            }


            for (int x = 0; x < src.rows(); x++)
            {
                for (int y = 0; y < src.cols(); y++)
                {
                    dst.data()[x][y] = ApplyBoxFilter(filter, src, x, y) / (kSize * kSize);
                }
            }
        }
    }

    virtual void filter2d(const Matrix &src, Matrix& dst, const Matrix &kernel)
    { }
    virtual void median(const Matrix &src, Matrix &dst, const int kSize = 3)
    {
        
    }
    virtual void SobelOx(const Matrix &src, Matrix &dst)
    { }

private:
    void formBorder(Matrix& src, int offset)
    {
        // upper bound
        for (int i = 0; i < offset; i++)
        {
            for (int j = 0; j < src.cols(); j++)
            {
                src.data()[i][j] = src.data()[i + offset][j];
            }
        }

        // lower bound
        for (int i = src.rows() - offset; i < src.rows(); i++)
        {
            for (int j = 0; j < src.cols(); j++)
            {
                src.data()[i][j] = src.data()[i - offset][j];
            }
        }
        
        // right bound
        for (int j = src.cols() - offset; j < src.cols(); j++)
        {
            for (int i = 0; i < src.rows(); i++)
            {
                src.data()[i][j] = src.data()[i][j - offset];
            }
        }

        // left bound
        for (int j = 0; j < offset; j++)
        {
            for (int i = 0; i < src.rows(); i++)
            {
                src.data()[i][j] = src.data()[i][j + offset];
            }
        }

        // corners
        //(left-up)
        for (int i = 0; i < offset; i++)
        {
            for (int j = i; j < offset; j++)
            {
                src.data()[i][j] = src.data()[i + offset][j + offset];
            }
        }

        //(left-bottom)
        for (int i = src.rows(); i >= src.rows() - offset; --i)
        {
            for (int j = 0; j < offset; j++)
            {

            }
        }

        //(right-up)
    }

    uchar ApplyBoxFilter(const Matrix& filter, const Matrix& src, int x, int y)
    {
        uchar pixel = 0;
        for (int i = 0; i < filter.rows(); i++)
        {
            for (int j = 0; j < filter.cols(); j++)
            {
                pixel += filter.data()[i][j] * src.data()[x + i][y + j];
            }
        }
        return pixel;
    }

    uchar ApplyMedianFilter()
    {

    }
};

Filters* createFiltersKozitsin()
{
    Filters* filters = new Filter_Kozitsin();
    return filters;
}