#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <string>
#include <sstream>
#include "parser.hpp"


class Visualizer {
    public:
        Visualizer() {
        }
        static std::string maskToStr(mask m) {
            unsigned int number = Parser::getIntFromMask(m);
            if (number > 0)
                return std::to_string(number);
            else
                return " ";
        }
        static std::string maskToStr(mask m, unsigned int optional) {
            return maskToStr(m);
        }
        static std::string printSlice(mask *slice) {
            std::string outString = "";
            outString += "| ";
            for (int i = 0; i < N; i++) {
                outString += maskToStr(slice[i]);
                outString += " ";
                if ((i + 1) % 3 == 0)
                    outString += "| ";
            }
            return outString;
        }
        static std::string maskToStrHighlight(mask m, unsigned int i) {
            bool isBitSet = Parser::isBitSet(m, i);
            if (isBitSet) {
                if (Parser::isOnlyOneBit(m)) {
                    return std::to_string(i);
                }
                else {
                    return " ";
                }
            }
            else {
                return "X";
            }
        }
        static std::string printBoard(mask *board) {
            return printBoardWithContent(board, maskToStr, 0);
        }
        static std::string printBoardHighlight(mask *board, unsigned int highlight) {
            return printBoardWithContent(board, maskToStrHighlight, highlight);
        }
        static std::string printBoardWithContent(
                mask *board,
                std::string (*printFcn)(mask, unsigned int),
                unsigned int optional
                ) {
            std::string outString = "";
            //DEBUG
            std::stringstream bb;
            bb << board;
            outString += "Board at : " + bb.str() + "\n";
            outString += "-------------------------\n";
            for (int i = 0; i < N*N; i++) {
                if (i % N == 0)
                    outString += "| ";
                outString += printFcn(board[i], optional);
                outString += " ";
                if ((i + 1) % 3 == 0)
                    outString += "| ";
                if ((i + 1) % N == 0)
                    outString += "\n";
                if ((i + 1) % (N*3) == 0)
                    outString += "-------------------------\n";
            }
            return outString;
        }
};

#endif
