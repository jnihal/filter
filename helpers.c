#include "helpers.h"
#include<math.h>

// Helper function
float limit(float x);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Find the average of the three colors
            int avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);

            // Assign the average to each color channel
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over each pixel array
    for (int i = 0; i < height; i++)
    {
        // Reverse the pixel array
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp;
            temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy the image to a temporary array
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    float sum_red, sum_green, sum_blue, count;

    // Loop over each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Reset the value after a pixel is blurred
            count = sum_red = sum_green = sum_blue = 0;

            // Loop over the neighbouring pixels
            for (int k = i - 1; k < i + 2; k++)
            {
                for (int l = j - 1; l < j + 2; l++)
                {
                    // Pixels that are outside the edge of the image
                    if (k < 0 || k >= height || l < 0 || l >= width)
                    {
                        continue;
                    }

                    // Calculate the sum of each color channel
                    sum_red += temp[k][l].rgbtRed;
                    sum_green += temp[k][l].rgbtGreen;
                    sum_blue += temp[k][l].rgbtBlue;
                    count++;
                }
            }

            // Assign the average to the original image to get the blurred pixel
            image[i][j].rgbtRed = round(sum_red / count);
            image[i][j].rgbtGreen = round(sum_green / count);
            image[i][j].rgbtBlue = round(sum_blue / count);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Initialize the Gx and Gy kernels
    int X[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Y[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Copy the image to a temporary array
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    // To store values along Gx direction
    float xsum_red, xsum_green, xsum_blue;

    // To store values along Gy direction
    float ysum_red, ysum_green, ysum_blue;

    // Loop over each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Reset the value
            xsum_red = xsum_green = xsum_blue = 0;
            ysum_red = ysum_green = ysum_blue = 0;

            // Loop over the neighbouring pixels
            for (int k = i - 1, m = 0; k < i + 2; k++, m++)
            {
                for (int l = j - 1, n = 0; l < j + 2; l++, n++)
                {
                    // Pixels that are outside the edge of the image
                    if (k < 0 || k >= height || l < 0 || l >= width)
                    {
                        continue;
                    }
                    else
                    {
                        // Calculate the Gx value for each color channel
                        xsum_red += temp[k][l].rgbtRed * X[m][n];
                        xsum_green += temp[k][l].rgbtGreen * X[m][n];
                        xsum_blue += temp[k][l].rgbtBlue * X[m][n];

                        // Calculate the Gy value for each color channel
                        ysum_red += temp[k][l].rgbtRed * Y[m][n];
                        ysum_green += temp[k][l].rgbtGreen * Y[m][n];
                        ysum_blue += temp[k][l].rgbtBlue * Y[m][n];
                    }
                }
            }

            // Combine the Gx and Gy values
            image[i][j].rgbtRed = round(limit(sqrt(pow(xsum_red, 2) + pow(ysum_red, 2))));
            image[i][j].rgbtGreen = round(limit(sqrt(pow(xsum_green, 2) + pow(ysum_green, 2))));
            image[i][j].rgbtBlue = round(limit(sqrt(pow(xsum_blue, 2) + pow(ysum_blue, 2))));
        }
    }
    return;
}

// Limit the value of the channel between 0 to 255
float limit(float x)
{
    if (x > 255)
    {
        return 255;
    }
    else if (x < 0)
    {
        return 0;
    }
    else
    {
        return x;
    }
}