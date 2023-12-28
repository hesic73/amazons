#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "core.h"
#include <cstdint>
#include <cmath>
#include <cstdlib>

// refer to https://wiki.botzone.org.cn/index.php?title=Amazons for convinience
// PS: it's a shame that C++ is without a generator until std 23.
static int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
static int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

constexpr bool inMap(int x, int y, int size)
{
    return (x >= 0 && x < size && y >= 0 && y < size);
}

namespace py = pybind11;

class Amazons_np : public Amazons
{
public:
    Amazons_np(std::uint32_t size) : Amazons(size)
    {
        int buffer_size = static_cast<int>(std::pow(size, 6));
        buffer = new int[buffer_size * 6];
    }
    ~Amazons_np()
    {
        delete[] buffer;
    }
    bool step(py::array_t<std::uint32_t> action)
    {
        auto buf_info = action.request();
        auto ptr = static_cast<std::uint32_t *>(buf_info.ptr);
        auto x0 = ptr[0];
        auto y0 = ptr[1];
        auto x1 = ptr[2];
        auto y1 = ptr[3];
        auto x2 = ptr[4];
        auto y2 = ptr[5];
        return Amazons::step(x0, y0, x1, y1, x2, y2);
    }
    py::array_t<int> get_board()
    {
        auto ptr = Amazons::get_board_view();
        auto arr = py::array_t<int>({size, size}, {sizeof(int) * size, sizeof(int)}, ptr);
        return arr;
    }

    py::array_t<int> random_action()
    {
        int cnt = 0;
        auto turn = get_turn();
        auto positions = get_current_positions();
        auto board_view = Amazons::get_board_view();
        auto board_size = static_cast<int>(size);

        for (int i = 0; i < 4; i++)
        {
            int x0 = static_cast<int>(positions[i][0]);
            int y0 = static_cast<int>(positions[i][1]);

            for (int j = 0; j < 8; j++)
            {
                for (int delta1 = 1; delta1 < board_size; delta1++)
                {
                    int x1 = x0 + delta1 * dx[j];
                    int y1 = y0 + delta1 * dy[j];
                    if (!inMap(x1, y1, board_size) || board_view[x1 * board_size + y1] != static_cast<int>(Cell::Empty))
                    {
                        break;
                    }
                    for (int k = 0; k < 8; k++)
                    {
                        for (int delta2 = 1; delta2 < board_size; delta2++)
                        {
                            int x2 = x1 + delta2 * dx[k];
                            int y2 = y1 + delta2 * dy[k];
                            if (!inMap(x2, y2, board_size) ||
                                (board_view[x2 * board_size + y2] != static_cast<int>(Cell::Empty)) &&
                                    !(x0 == x2 && y0 == y2))
                            {
                                break;
                            }
                            auto offset = cnt * 6;
                            buffer[offset + 0] = x0;
                            buffer[offset + 1] = y0;
                            buffer[offset + 2] = x1;
                            buffer[offset + 3] = y1;
                            buffer[offset + 4] = x2;
                            buffer[offset + 5] = y2;
                            cnt++;
                        }
                    }
                }
            }
        }

        auto idx = std::rand() % cnt;
        auto raw_ptr = new int[6];
        std::memcpy(raw_ptr, buffer + idx * 6, 6 * sizeof(int));
        return py::array_t<int>({6}, raw_ptr);
    }

    py::memoryview get_board_view()
    {
        auto ptr = Amazons::get_board_view();
        auto view = py::memoryview::from_buffer(ptr, {size, size}, {sizeof(int) * size, sizeof(int)});
        return view;
    }

private:
    int *buffer;
};

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

PYBIND11_MODULE(_amazons, m)
{

    auto amazons = py::class_<Amazons_np>(m, "Amazons");

    amazons.def(py::init<std::uint32_t>())
        .def("reset", &Amazons_np::reset)
        .def("step", &Amazons_np::step, py::arg("action"))
        .def_property("done", &Amazons_np::is_done, nullptr)
        .def("get_board_view", &Amazons_np::get_board_view)
        .def("get_board", &Amazons_np::get_board)
        .def("string_repr", &Amazons_np::string_repr)
        .def("random_action", &Amazons_np::random_action);

    py::enum_<Cell>(amazons, "Cell")
        .value("Empty", Cell::Empty)
        .value("Black", Cell::Black)
        .value("White", Cell::White)
        .value("Arrow", Cell::Arrow)
        .export_values();

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}