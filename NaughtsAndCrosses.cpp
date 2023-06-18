#include <iostream>

/*
 *  размер игрового поля в ячейках
 */
int boardSize = 3;
/*
 * строка для хранения состояния поля
 */
std::string board;

/*
 * функция преобразует координаты ячейки поля в позицию символа строки board
 * x - колонка
 * y - строка
 */
int coordinate2position(int x, int y) {
    return y*boardSize+x;
}

/*
 * функция проверяет корректность ввода строки
 */
bool check_line(std::string line) {
    if (line.length()==boardSize) {
        for (int i=0; i<boardSize; i++) {
            if (!(line[i] == 'X' || line[i] == 'O' || line[i] == '.')) {
                return false;
            }
        }
        return true;
    }
    return false;
}

/*
 * функция осуществляет ввод строки
 * nRow - номер вводимой строки используется для вывода приглашения ко вводу
 */
std::string input_line(int nRow) {
    std::string result;
    do {
        std::cout << "Input line #" << nRow << ": ";
        std:: cin >> result;
    } while (!check_line(result));
    return result;
}

/*
 * Функция осуществляет подсчёт ходов сделанных одним игроком
 * с - подсчитываемые ходы 'X' или 'O'. Проверка параметра не осуществляется
 */
int calc_moves(char c) {
    int result=0;
    for (int i=0; i<board.length(); i++) {
        if (board[i]==c) result++;
    }
    return result;
}

/*
 * x - колонка
 * y - строка
 * board - игровое поле
 */
char char_in_position(int x, int y) {
  return board[coordinate2position(x, y)];
}

/*
 * служебная функция возвращает true, если диагональ выигрышная
 * board - игровое поле
 * move - ход в левый верхний угол игрового поля
 */
bool check_diagonal_LR(char move) {
    //i=0 позволяет выполнить проверку второй диагонали на то же значение move
    for (int i = 0; i < boardSize; i++) {
        if (char_in_position(i,i) != move) return false;
    }
    return true;
}

/*
 * служебная функция возвращает true, если диагональ выигрышная
 * board - игровое поле
 * move - ход в правый верхний угол игрового поля
 */
bool check_diagonal_RL(char move) {
    //i=0 позволяет выполнить проверку второй диагонали на то же значение move
    for (int i = 0; i < boardSize; i++) {
        if (char_in_position(boardSize-1-i,i) != move) return false;
    }
    return true;
}
/*
 * функция возвращает победителя по одной из диагоналей или '.', если победителя нет,
 * или 'E', если обе диагонали "выиграли"
 */
char check_diagonals() {
    char result='E';
    result = char_in_position(0, 0);
    if (result != '.') {
        if (check_diagonal_LR(result)) {
            return (check_diagonal_RL(result)) ? 'E' : result;
        }
    }
    result = char_in_position(boardSize-1, 0);
    if (result != '.' && !check_diagonal_RL(result)) result = '.';
    return result;
}

/*
 * если move='.' функция возвращает победителя в колонке или '.'
 * если move!='.' функция возвращает 'E' если найдена выигрышная колонка или '.' если такая колонка не найдена
 * board - игровое поле
 * move - move=='.' если ещё не был найден победитель игры, иначе move содержит 'X', или 'O'
 */
char check_columns(char move) {
    char result='E';
    char c;
    for (int x=0; x<boardSize; x++) {
        result = char_in_position(x, 0);
        // если move != '.', то уже был определён победитель (диагональ),
        // следовательно, победная колонка должна содержать только символы move
        if (!(result=='.' || result == move)) {
            for (int y = 1; y < boardSize; y++) {
                c=char_in_position(x, y);
                if (c == '.' || c != result) {
                    result = '.';
                    break;
                }
            }
            if (result != '.') {
                if (move != '.') result='E';
                break;
            }
        }
    }
    return result;
}

/*
 * если move='.' функция возвращает победителя в строке или '.'
 * если move!='.' функция возвращает 'E' если найдена выигрышная строка или '.' если такая строка не найдена
 * board - игровое поле
 * move - move=='.' если ещё не был найден победитель игры, иначе move содержит 'X', или 'O'
 */
char check_rows(char move) {
    char result='E';
    char c;
    for (int y=0; y<boardSize; y++) {
        result = char_in_position(0, y);
        // если move != '.', то уже был определён победитель (диагональ, колонка),
        // следовательно, победная колонка должна содержать только символы move
        if (!(result=='.' || result == move)) {
            for (int x = 1; x < boardSize; x++) {
                c=char_in_position(x, y);
                if (c == '.' || c != result) {
                    result = '.';
                    break;
                }
            }
            if (result != '.') {
                if (move != '.') result='E';
                break;
            }
        }
    }
    return result;
}

char get_winner() {
    char winner;
    winner = check_diagonals();
    if (winner == 'E') return winner;
    winner = check_columns(winner);
    if (winner == 'E') return winner;
    return check_rows(winner);
}

int main() {
    std::cout << "\ntic-tac-toe\n\n!" << std::endl;

    int crosses = 0, zeroes = 0;
    board = "";
    for (int i = 1; i <= boardSize; i++) {
        board += input_line(i);
    }
    if (board.length() != boardSize * boardSize) return -1;

    crosses = calc_moves('X');
    zeroes = calc_moves('O');

    if (!(crosses == zeroes || crosses == zeroes + 1)) {
        std::cout << "Incorrect" << std::endl;
        return 0;
    }
    if (crosses < boardSize) {
        std::cout << "Nobody" << std::endl;
        return 0;
    }

    char winner = get_winner();

    if (winner=='.') {
        std::cout << "Nobody" << std::endl;
    } else if (winner == 'X' && crosses == zeroes + 1) {
        std::cout << "Petya won" << std::endl;
    } else if (winner == 'O' && crosses == zeroes) {
        std::cout << "Vasya won" << std::endl;
    } else {
        std::cout << "Incorrect" << std::endl;
    }

}
