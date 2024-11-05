#include <pybind11/pybind11.h>
#include "salt/salt.h"
#include "salt/base/eval.h"
#include <string>

namespace py = pybind11;

void net_file(const std::string& netFile, double eps) {
    salt::Net net;
    net.Read(netFile);
    printlog("Run SALT algorithm on net", net.name, "with", net.pins.size(), "pins using epsilon =", eps);

    // Run SALT
    salt::Tree tree;
    salt::SaltBuilder saltB;
    saltB.Run(net, tree, eps);

    // Report
    printlog("Tree topology is as follows:");
    cout << tree;
    salt::WireLengthEval eval(tree);
    printlog("Wire length is", eval.wireLength);
    printlog("Max path length is", eval.maxPathLength);
    printlog("Avg path length is", eval.avgPathLength);
    printlog("Max stretch (shallowness) is", eval.maxStretch);
    printlog("Avg stretch is", eval.avgStretch);
    tree.Write("SALT");
}

PYBIND11_MODULE(pysaltrouting, m) {
    m.doc() = "python binding for salt";
    m.def("net_file", &net_file, "routing net file", py::arg("netFile"), py::arg("eps") = 1.101);
} 
