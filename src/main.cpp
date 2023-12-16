#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

void free2DMatrixFromMemory(double **matrix, int H)
{
    for(int i = 0 ; i < H ; i++)
    {
        delete [] matrix[i];
    }
    delete[] matrix;
    matrix = nullptr;
}

double** calculateEnergyMatrix(int ***rgb, int H, int W)
{
    double** energyMatrix = new double*[H];
    for(int i = 0 ; i < H ; i++)
    {
        energyMatrix[i] = new double[W];
    }
    
    for(int height = 0 ; height < H ; height++)
    {
        for(int width = 0; width < W; width++)
        {
            int x_plus_one = ((width + 1) == W) ? 0 : (width+1);
            int x_minus_one = ((width - 1) < 0) ? (W-1) : (width - 1);
            int y_plus_one = ((height + 1) == H) ? 0 : (height + 1);
            int y_minus_one = ((height - 1) < 0) ? (H - 1) : (height - 1);
            
            double rx = rgb[height][x_plus_one][0] - rgb[height][x_minus_one][0];
            double gx = rgb[height][x_plus_one][1] - rgb[height][x_minus_one][1];
            double bx = rgb[height][x_plus_one][2] - rgb[height][x_minus_one][2];
            double delX = rx * rx + gx * gx + bx * bx;
            
            double ry = rgb[y_plus_one][width][0] - rgb[y_minus_one][width][0];
            double gy = rgb[y_plus_one][width][1] - rgb[y_minus_one][width][1];
            double by = rgb[y_plus_one][width][2] - rgb[y_minus_one][width][2];
            double delY = ry * ry + gy * gy + by * by;
            
            energyMatrix[height][width] = sqrt(delX + delY);
        }
    }
    return energyMatrix;
}

double** calculateVerticalPathMatrix(double** energyMatrix, int H, int W)
{
    double **pathMatrix = new double*[H];
    for(int i = 0 ; i < H ; i++)
    {
        pathMatrix[i] = new double[W];
    }
    // Initialize the pathMatrix
    for(int h = 0 ; h < H; h++)
    {
        for(int w = 0 ; w < W; w++)
        {
            pathMatrix[h][w] = INT_MAX;
        }
    }
    
    for(int i = 0 ; i < W; i++)
    {
        pathMatrix[0][i] = energyMatrix[0][i];
    }
    
    for(int height = 0 ; height < H - 1 ; height++)
    {
        for(int width = 0 ; width < W; width++)
        {
            int left = max(0, width - 1), center = width, right = min(width + 1, W - 1);
            pathMatrix[height + 1][left] = min(pathMatrix[height + 1][left], pathMatrix[height][width] + energyMatrix[height + 1][left]);
            pathMatrix[height + 1][center] = min(pathMatrix[height + 1][center], pathMatrix[height][width] + energyMatrix[height + 1][center]);
            pathMatrix[height + 1][right] = min(pathMatrix[height + 1][right], pathMatrix[height][width] + energyMatrix[height + 1][right]);
        }
    }
    
    return pathMatrix;
}

double** calculateHorizontalPathMatrix(double** energyMatrix, int H, int W)
{
    double **pathMatrix = new double*[H];
    for(int i = 0 ; i < H ; i++)
    {
        pathMatrix[i] = new double[W];
    }
    // Initialize the pathMatrix
    for(int h = 0 ; h < H; h++)
    {
        for(int w = 0 ; w < W; w++)
        {
            pathMatrix[h][w] = INT_MAX;
        }
    }
    
    for(int i = 0 ; i < H; i++)
    {
        pathMatrix[i][0] = energyMatrix[i][0];
    }
    
    for(int w = 0 ; w < W - 1; w++)
    {
        for(int h = 0 ; h < H; h++)
        {
            int up = max(0, h - 1), center = h, down = min(h + 1, H - 1);
            pathMatrix[up][w + 1] = min(pathMatrix[up][w + 1], pathMatrix[h][w] + energyMatrix[up][w + 1]);
            pathMatrix[center][w + 1] = min(pathMatrix[center][w + 1], pathMatrix[h][w] + energyMatrix[center][w + 1]);
            pathMatrix[down][w + 1] = min(pathMatrix[down][w + 1], pathMatrix[h][w] + energyMatrix[down][w + 1]);
            
        }
    }
    
    
    return pathMatrix;
}

void solve(int ***rgb, int H, int W, int C, int H_, int W_, int C_) {
    
    int heightToReduce = H - H_, widthToReduce = W - W_, currentHeight = H, currentWidth = W;
    
    // Removing vertical seams
    for(int i = 0 ; i < widthToReduce; i++)
    {
        double **energyMatrix = calculateEnergyMatrix(rgb, currentHeight, currentWidth);
        double **pathMatrix = calculateVerticalPathMatrix(energyMatrix, currentHeight, currentWidth);
        double minValue = INT_MAX;
        int minIndex = -1;
        for(int w = 0 ; w < currentWidth; w++)
        {
            if(minValue > pathMatrix[currentHeight - 1][w])
            {
                minValue = pathMatrix[currentHeight - 1][w];
                minIndex = w;
            }
        }
        
        // shifting columns of last row of rgb matrix
        for(int i = minIndex; i < currentWidth - 1; i++)
        {
            rgb[currentHeight - 1][i] = rgb[currentHeight - 1][i + 1];
        }
        // now finding min column value for all above rows
        for(int h = currentHeight - 1; h > 0; h--)
        {
            int left = max(0, minIndex - 1), center = minIndex, right = min(minIndex + 1, currentWidth - 1);
            if(pathMatrix[h][minIndex] == (pathMatrix[h - 1][left] + energyMatrix[h][minIndex]))
            {
                minIndex = left;
                
            }
            if(pathMatrix[h][minIndex] == (pathMatrix[h - 1][center] + energyMatrix[h][minIndex]))
            {
                minIndex = center;
                
            }
            if(pathMatrix[h][minIndex] == (pathMatrix[h - 1][right] + energyMatrix[h][minIndex]))
            {
                minIndex = right;
                
            }
            // upper row correct column found. Now shift columns of rgb matrix for upper row
            for(int i = minIndex; i < currentWidth - 1; i++)
            {
                rgb[h - 1][i] = rgb[h - 1][i + 1];
            }
        }
        currentWidth--;
        free2DMatrixFromMemory(energyMatrix, currentHeight);
        free2DMatrixFromMemory(pathMatrix, currentHeight);
    }
    
    for(int i = 0 ; i < heightToReduce; i++)
    {
        double **energyMatrix = calculateEnergyMatrix(rgb, currentHeight, currentWidth);
        double **pathMatrix = calculateHorizontalPathMatrix(energyMatrix, currentHeight, currentWidth);
        
        int minValue = INT_MAX, minIndex = -1;
        for(int h = 0 ; h < currentHeight; h++)
        {
            if(minValue > pathMatrix[h][currentWidth - 1])
            {
                minValue = pathMatrix[h][currentWidth - 1];
                minIndex = h;
            }
        }
        //shift last column of rgb matrix
        for(int h = minIndex; h < currentHeight - 1; h++)
        {
            rgb[h][currentWidth - 1] = rgb[h + 1][currentWidth - 1];
        }
        // now find correct height values for all widths
        for(int w = currentWidth - 1; w > 0; w--)
        {
            int up = max(0, minIndex - 1), center = minIndex, down = min(minIndex + 1, currentHeight - 1);
            if(pathMatrix[minIndex][w] == (pathMatrix[up][w - 1] + energyMatrix[minIndex][w]))
            {
                minIndex = up;
            }
            if(pathMatrix[minIndex][w] == (pathMatrix[center][w - 1] + energyMatrix[minIndex][w]))
            {
                minIndex = center;
            }
            if(pathMatrix[minIndex][w] == (pathMatrix[down][w - 1] + energyMatrix[minIndex][w]))
            {
                minIndex = down;
            }
            // shift rows of rgb of column w - 1
            for(int h = minIndex; h < currentHeight - 1; h++)
            {
                rgb[h][w - 1] = rgb[h + 1][w - 1];
            }
        }
        currentHeight -- ;
        free2DMatrixFromMemory(energyMatrix, currentHeight);
        free2DMatrixFromMemory(pathMatrix, currentHeight);
        
    }
    
    
}

int main() {
    string ip_dir = "./data/input/";
    string ip_file = "rgb_in.txt";
    ifstream fin(ip_dir + ip_file);
    
    int H, W, C;
    fin >> H >> W >> C;
    
    int ***rgb;
    rgb = new int **[H];
    for(int i = 0; i < H; ++i) {
        rgb[i] = new int *[W];
        for(int j = 0; j < W; ++j) {
            rgb[i][j] = new int[C];
            for(int k = 0; k < C; ++k)
                fin >> rgb[i][j][k];
        }
    }
    fin.close();
    
    int H_, W_, C_;
    cout << "Enter new value for H (must be less than " << H << "): ";
    cin >> H_;
    cout << "Enter new value for W (must be less than " << W << "): ";
    cin >> W_;
    cout << '\n';
    C_ = C;
    
    solve(rgb, H, W, C, H_, W_, C_);
    
    string op_dir = "./data/output/";
    string op_file = "rgb_out.txt";
    ofstream fout(op_dir + op_file);
    
    fout << H_ << " " << W_ << " " << C_ << endl;
    for(int i = 0; i < H_; ++i) {
        for(int j = 0; j < W_; ++j) {
            for(int k = 0; k < C_; ++k) {
                fout << rgb[i][j][k] << " ";
            }
        }
        fout << '\n';
    }
    fout.close();
    
    return 0;
}
