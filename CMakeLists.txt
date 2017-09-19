cmake_minimum_required(VERSION 3.8)

project(word2vec)

add_executable(word2vec word2vec.c)
add_executable(distance distance.c)
add_executable(word2phrase word2phrase.c)
add_executable(word-analogy word-analogy.c)
add_executable(compute-accuracy compute-accuracy.c)
add_executable(bin2text bin2text.c)
