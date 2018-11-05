#include <iostream>
#include <iomanip>


void solv_hanoi(int n_pieces, long long &n_moves,
                char source, char destination, char spare) {
    if (n_pieces == 1) {
        ++n_moves;
        std::cout << std::setw(4) << n_moves
                  << ". Move the top disk from " << source
                  << " to " << destination << std::endl;
    } else {
        solv_hanoi(n_pieces - 1, n_moves, source, spare, destination);
        solv_hanoi(1, n_moves, source, destination, spare);
        solv_hanoi(n_pieces - 1, n_moves, spare, destination, source);
    }

}

int main() {
    const char pillar_a = 'A', pillar_b = 'B', pillar_c = 'C';
    long long n_moves = 0;
    int n_pieces;

    std::cout << "Enter the number of disks:" << std::endl;
    std::cin >> n_pieces;
    std::cout << std::string(10, '=') 
              << " Solution " 
              << std::string(10, '=') 
              << std::endl;

    solv_hanoi(n_pieces, n_moves, pillar_a, pillar_c, pillar_b);

    return 0;
}
