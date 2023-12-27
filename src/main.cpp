#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "core.h"
#include <cstdint>

namespace py = pybind11;

class Amazons_np : public Amazons
{
public:
    Amazons_np(std::uint32_t size) : Amazons(size) {}
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

    py::memoryview get_board_view()
    {
        auto ptr = Amazons::get_board_view();
        auto view = py::memoryview::from_buffer(ptr, {size, size}, {sizeof(int) * size, sizeof(int)});
        return view;
    }
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
        .def("string_repr", &Amazons_np::string_repr);

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