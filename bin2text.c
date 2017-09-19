//  Copyright 2013 Google Inc. All Rights Reserved.
//  Copyright 2017 Leo Jiang. All Rights Reserved.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const long long max_size = 2000;         // max length of strings
const long long N = 40;                  // number of closest words that will be shown
const long long max_w = 50;              // max length of vocabulary entries

int main(int argc, char **argv) {
    int retval = -1;
    FILE *fin = NULL;
    FILE *fout = NULL;
    char *bestw[N];
    float len;
    long long words, size, a, b;
    float *M = NULL;
    char *vocab = NULL;

    if (argc < 3) {
        printf("Usage: ./bin2text <INPUT> <OUTPUT>\n"
               "where INPUT contains word projections in the BINARY FORMAT\n"
               "OUTPUT save int the TEXT FORMAT");
        return 0;
    }

    fin = fopen(argv[1], "rb");
    if (fin == NULL) {
        printf("Input file not found\n");
        goto cleanup;
    }

    fout = fopen(argv[2], "w");
    if (fout == NULL) {
        printf("Output file create failed\n");
        goto cleanup;
    }

    fscanf(fin, "%lld", &words);
    fscanf(fin, "%lld", &size);
    vocab = (char *)malloc((long long)words * max_w * sizeof(char));
    for (a = 0; a < N; a++) bestw[a] = (char *)malloc(max_size * sizeof(char));
    M = (float *)malloc((long long)words * (long long)size * sizeof(float));
    if (M == NULL) {
        printf("Cannot allocate memory: %lld MB    %lld  %lld\n", (long long)words * size * sizeof(float) / 1048576, words, size);
        return -1;
    }
    for (b = 0; b < words; b++) {
        a = 0;
        while (1) {
            vocab[b * max_w + a] = fgetc(fin);
            if (feof(fin) || (vocab[b * max_w + a] == ' ')) break;
            if ((a < max_w) && (vocab[b * max_w + a] != '\n')) a++;
        }
        vocab[b * max_w + a] = 0;
        for (a = 0; a < size; a++) fread(&M[a + b * size], sizeof(float), 1, fin);
        len = 0;
        for (a = 0; a < size; a++) len += M[a + b * size] * M[a + b * size];
        len = sqrt(len);
        for (a = 0; a < size; a++) M[a + b * size] /= len;
    }

    for (a = 0; a < words; a++) {
        fprintf(fout, "%s ", &vocab[a * max_w]);
        for (b = 0; b < size; b++) {
            if (b != size - 1)
                fprintf(fout, "%f ", M[a * size + b]);
            else
                fprintf(fout, "%f", M[a * size + b]);
        }
        fprintf(fout, "\n");
    }

    retval = 0;

cleanup:
    if (vocab != NULL) {
        free(vocab);
    }
    if (M != NULL) {
        free(M);
    }
    if (fin != NULL) {
        fclose(fin);
    }
    if (fout != NULL) {
        fclose(fout);
    }
    return retval;
}
