#include <vector>
#include <iostream>

using namespace std;
typedef unsigned char uchar;

class Matrix
{
public:
	int row;
	int col;
	vector<vector<uchar>> data;

	Matrix(int row, int col)
	{
		this->row = row;
		this->col = col;

		data.resize(row);
		for (int i = 0; i < row; i++)
		{
			data[i].resize(col);
		}
	}

	void Ones()
	{
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				data[i][j] = 1;
			}
		}
	}
};

static class Filter_Kozitsin
{
public:
	static void boxFilter(const Matrix &src, Matrix& dst, const int kSize = 3)
	{
		if (src.col < kSize || src.row < kSize)
		{
			return;
		}
		else
		{
			Matrix filter(kSize, kSize);
            filter.Ones();

			int offset = floor(kSize / 2.0);
			Matrix ext(src.row + offset * 2, src.col + offset * 2);

			#pragma region fill main body of matrix
			for (int i = offset; i < ext.row - offset; i++)
			{
				for (int j = offset; j < ext.col - offset; j++)
				{
					ext.data[i][j] = src.data[i - offset][j - offset]; 
				}
			}
			#pragma endregion
			border(ext, offset);

			for (int x = offset; x < ext.row - offset; x++)
            {
                for (int y = offset; y < ext.col - offset; y++)
                {
                    ext.data[x][y] = ApplyBoxFilter(filter, src, x, y) / (kSize * kSize);
                }
            }
			for (int i = 0; i < ext.row; i++)
			{
				for (int j = 0; j < ext.col; j++)
				{
					cout << (int)ext.data[i][j] << ' ';
				}
				cout << endl;
			}
			cout << "===============\n";
			for (int i = 0; i < dst.row; i++)
			{
				for (int j = 0; j < dst.col; j++)
				{
					dst.data[i][j] = ext.data[i + offset][j + offset];
				}
			}
		}
	}

	static void border(Matrix& src, int offset)
	{
		// upper bound
		for (int i = 0; i < offset; i++)
		{
			for (int j = 0; j < src.col; j++)
			{
				src.data[i][j] = src.data[i + offset][j];
			}
		}

		// lower bound
		for (int i = src.row - offset; i < src.row; i++)
		{
			for (int j = 0; j < src.col; j++)
			{
				src.data[i][j] = src.data[i - offset][j];
			}
		}

		// right bound
		for (int j = src.col - offset; j < src.col; j++)
		{
			for (int i = 0; i < src.row; i++)
			{
				src.data[i][j] = src.data[i][j - offset];
			}
		}

		// left bound
		for (int j = 0; j < offset; j++)
		{
			for (int i = 0; i < src.row; i++)
			{
				src.data[i][j] = src.data[i][j + offset];
			}
		}

		/*
		// corners
		//(left-up)
		for (int i = 0; i < offset; i++)
		{
			for (int j = i; j < offset; j++)
			{
				src.data[i][j] = src.data[i + offset][j + offset];
			}
		}
		
		//(left-bottom)
		for (int i = src.row; i >= src.row - offset; --i)
		{
			for (int j = src.col - i; j < offset; j++)
			{
				src.data[i][j] = src.data[i
			}
		}
		*/
		//(right-up)
	}

	static uchar ApplyBoxFilter(const Matrix& filter, const Matrix& src, int x, int y)
    {
        uchar pixel = 0;
        for (int i = 0; i < filter.row; i++)
        {
            for (int j = 0; j < filter.col; j++)
            {
				if (x + i < src.row && y + j < src.col)
					pixel += filter.data[i][j] * src.data[x + i][y + j];
            }
        }
        return pixel;
    }
};

int main()
{
	Matrix src(6, 6);
	Matrix dst(6, 6);

	int temp = 0;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			src.data[i][j] = temp++;
		}
	}

	for (int i = 0; i < src.row; i++)
	{
		for (int j = 0; j < src.col; j++)
		{
			cout << (int)src.data[i][j] << ' ';
		}
		cout << endl;
	}
	cout << "================" << endl;
	Filter_Kozitsin::boxFilter(src, dst);

	for (int i = 0; i < dst.row; i++)
	{
		for (int j = 0; j < dst.col; j++)
		{
			cout << (int)dst.data[i][j] << ' ';
		}
		cout << endl;
	}
}
