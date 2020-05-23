#include "FeedHandler.h"

#include <boost/program_options.hpp>

#include <fstream>
#include <string>

namespace po = boost::program_options;

namespace {

inline uint64_t rdtsc() {
    uint32_t hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) + (uint64_t)lo;
}

class Profiler {
    double m_mean{};
    size_t m_count{};

public:
    Profiler() = default;
    
    ~Profiler() {
        std::cout << "mean latency: " << m_mean << ", count:" << m_count << std::endl;
    }
    
    void add(uint32_t cycles) {
        m_mean = (m_mean * m_count + cycles) / static_cast<double>(m_count + 1);
        m_count += 1;
    }
};

}

int main(int argc, char **argv) {
    std::string input_file;
    bool print_book;

    po::options_description desc("Allowed Options");
    desc.add_options()
        ("help,h", "print usage")
        ("input,i", po::value(&input_file), "input file")
        ("print-book", po::value(&print_book)->default_value(false), "print book")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    impl::FeedHandler feed_handler{};
    std::string line;

    std::ifstream infile(input_file.c_str(), std::ios::in);

    int counter = 0;
    Profiler profiler;

    while (std::getline(infile, line)) {
        try {
            auto start = rdtsc();
            feed_handler.process(line);
            auto end = rdtsc();
            
            if (++counter % 10 == 0 && print_book) {
                feed_handler.print_book(std::cout);
            }

            profiler.add(end - start);
        } catch (std::runtime_error const&) {}
    }

    return 0;
}
