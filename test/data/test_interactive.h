#ifndef __NODE_0_MODULE__
#define __NODE_0_MODULE__

#include <string>
#include <systemc>
using namespace sc_core;

class Node_0_module : public sc_module
{
private:
    uint64_t no_ins, no_outs;
    void set_sysc_output();
public:
    uint64_t no_inputs, no_outputs;
    sc_fifo<sc_dt::sc_bv<32>>* sysc_output;
    int get_no_inputs() const;
    int get_no_outputs() const;
    void body();
    SC_HAS_PROCESS(Node_0_module);
    Node_0_module(sc_module_name name);
};
#endif
#ifndef __NODE_1_MODULE__
#define __NODE_1_MODULE__

#include <string>
#include <systemc>
using namespace sc_core;

class Node_1_module : public sc_module
{
private:
    uint64_t no_ins, no_outs;
    void set_sysc_output();
public:
    uint64_t no_inputs, no_outputs;
    sc_fifo<sc_dt::sc_bv<32>>* sysc_output;
    int get_no_inputs() const;
    int get_no_outputs() const;
    void body();
    SC_HAS_PROCESS(Node_1_module);
    Node_1_module(sc_module_name name);
};
#endif
#ifndef __ADD_2_MODULE__
#define __ADD_2_MODULE__

#include <string>
#include <systemc>
using namespace sc_core;
#include "Python.h"

class Add_2_module : public sc_module
{
private:
    PyObject *pBody, *pName, *pModule, *pyC, *pExit;
    uint64_t no_ins, no_outs;
    PyObject* type_sysc_a;
    PyObject* get_sysc_a();
    sc_dt::sc_bv<32> bits_sysc_a;
    PyObject* type_sysc_b;
    PyObject* get_sysc_b();
    sc_dt::sc_bv<32> bits_sysc_b;
    PyObject* type_sysc_output;
    sc_dt::sc_bv<32> bits_sysc_output;
    void set_sysc_output();
public:
    uint64_t no_inputs, no_outputs;
    sc_fifo<sc_dt::sc_bv<32>>* sysc_a;
    sc_fifo<sc_dt::sc_bv<32>>* sysc_b;
    sc_fifo<sc_dt::sc_bv<32>>* sysc_output;
    int get_no_inputs() const;
    int get_no_outputs() const;
    void body();
    SC_HAS_PROCESS(Add_2_module);
    Add_2_module(sc_module_name name);
};
#endif
#ifndef __INTERACTIVE_3_MODULE__
#define __INTERACTIVE_3_MODULE__

#include <string>
#include <systemc>
using namespace sc_core;
#include "Python.h"

class Interactive_3_module : public sc_module
{
private:
    PyObject *pBody, *pName, *pModule, *pyC, *pExit;
    PyObject *runtimeModule, *pNode;
    static PyObject* sc_receive(PyObject *self, PyObject *args);
    static PyObject* sc_send(PyObject *self, PyObject *args);
    static PyObject* PyInit_sysc(void);
    static PyMethodDef SysCMethods[];
    static PyModuleDef SysCModule;
    static Interactive_3_module* singleton;
    void init();
    void store();
    void run();
    uint64_t no_ins, no_outs;
    PyObject* type_sysc_num;
    PyObject* get_sysc_num();
    sc_dt::sc_bv<32> bits_sysc_num;
    PyObject* type_sysc_output;
    sc_dt::sc_bv<32> bits_sysc_output;
    void set_sysc_output();
public:
    uint64_t no_inputs, no_outputs;
    sc_fifo<sc_dt::sc_bv<32>>* sysc_num;
    sc_fifo<sc_dt::sc_bv<32>>* sysc_output;
    int get_no_inputs() const;
    int get_no_outputs() const;
    void body();
    SC_HAS_PROCESS(Interactive_3_module);
    Interactive_3_module(sc_module_name name);
};
#endif
#ifndef __PRINT_THEN_EXIT_4_MODULE__
#define __PRINT_THEN_EXIT_4_MODULE__

#include <string>
#include <systemc>
using namespace sc_core;
#include "Python.h"

class Print_Then_Exit_4_module : public sc_module
{
private:
    PyObject *pBody, *pName, *pModule, *pyC, *pExit;
    uint64_t no_ins, no_outs;
    PyObject* type_sysc_n;
    PyObject* get_sysc_n();
    sc_dt::sc_bv<32> bits_sysc_n;
public:
    uint64_t no_inputs, no_outputs;
    sc_fifo<sc_dt::sc_bv<32>>* sysc_n;
    int get_no_inputs() const;
    int get_no_outputs() const;
    void body();
    SC_HAS_PROCESS(Print_Then_Exit_4_module);
    Print_Then_Exit_4_module(sc_module_name name);
};
#endif

#ifndef __TEST_INTERACTIVE__
#define __TEST_INTERACTIVE__
#include <systemc>
using namespace sc_core;
using namespace sc_dt;

// TODO: Find better way of handling trace files
sc_trace_file *Tf;

// Converts clock signals to bit vectors for Migen nodes
SC_MODULE(ClkToBV) {
    sc_in<bool> clk;
    sc_out<sc_bv<1>> clkout;

    void run() {
        clkout.write(clk.read());
    }

    SC_CTOR(ClkToBV) {
        SC_METHOD(run);
        sensitive << clk;
    }
};

// Adaptor for going from Python to Migen
template <class T> SC_MODULE(PythonToMigen) {
    sc_in<bool> clk;
    sc_out<T> migen_data_out;
    sc_out<sc_bv<1>> migen_valid_out;
    sc_in<sc_bv<1>> migen_ready_in;
    sc_fifo<T>* py_in;

    void run() {
        if (migen_ready_in.read() == 1) {
            T val;
            if (py_in->nb_read(val)) {
                migen_data_out.write(val);
                migen_valid_out.write(1);
            } else {
                migen_valid_out.write(0);
            }
        }
    }

    SC_CTOR(PythonToMigen) {
        SC_METHOD(run);
        sensitive << clk.pos();
    }
};

// Adaptor for going from Migen to Python
template <class T> SC_MODULE(MigenToPython) {
    sc_in<bool> clk;
    sc_in<T> migen_in;
    sc_in<sc_bv<1>> migen_valid_in;
    sc_out<sc_bv<1>> migen_ready_out;
    sc_fifo<T>* py_out;

    void run() {
        while (true) {
            wait();
            if (py_out->num_free() > 0) {
                migen_ready_out.write(1);
                if (migen_valid_in.read() == 1) {
                    py_out->write(migen_in.read());
                }
            } else {
                migen_ready_out.write(0);
            }
        }
    }

    SC_CTOR(MigenToPython) {
        SC_THREAD(run);
        sensitive << clk.pos();
    }
};

SC_MODULE(Test_Interactive){

// Python nodes to Python nodes just need a queue
sc_fifo<sc_dt::sc_bv<32>> wire_0_0_2_0;
sc_fifo<sc_dt::sc_bv<32>> wire_1_0_2_1;
sc_fifo<sc_dt::sc_bv<32>> wire_2_0_3_0;
sc_fifo<sc_dt::sc_bv<32>> wire_3_0_4_0;

// Migen nodes to Migen nodes need data wires,
// as well as valid and ready signals

// Python to Migen need an adaptor which maps a queue
// to data, valid and ready signals.

// Migen to Python need an adaptor which maps signals
// to a queue

// Python to template need a public queue

// Template to python need a public queue

// Migen to template need public data, valid and ready signals

// Template to Migen need public data, valid and ready signals

sc_in<bool> clk, rst;
sc_signal<sc_dt::sc_bv<1>> rst_bv;

// Node modules
Node_0_module node_0;
Node_1_module node_1;
Add_2_module add_2;
Interactive_3_module interactive_3;
Print_Then_Exit_4_module print_then_exit_4;


// Constructor first initialises queues, modules and adaptors
typedef Test_Interactive SC_CURRENT_USER_MODULE;
Test_Interactive(sc_module_name name, sc_trace_file *Tf):
wire_0_0_2_0("wire_0_0_2_0"),
wire_1_0_2_1("wire_1_0_2_1"),
wire_2_0_3_0("wire_2_0_3_0"),
wire_3_0_4_0("wire_3_0_4_0"),
node_0("node_0"),
node_1("node_1"),
add_2("add_2"),
interactive_3("interactive_3"),
print_then_exit_4("print_then_exit_4")
{
    SC_METHOD(rstprop);
    sensitive << rst;

    // Wiring the clock to the Migen nodes

    // Wiring the Python to Python nodes
    node_0.sysc_output = &wire_0_0_2_0;
    add_2.sysc_a = &wire_0_0_2_0;
    node_1.sysc_output = &wire_1_0_2_1;
    add_2.sysc_b = &wire_1_0_2_1;
    add_2.sysc_output = &wire_2_0_3_0;
    interactive_3.sysc_num = &wire_2_0_3_0;
    interactive_3.sysc_output = &wire_3_0_4_0;
    print_then_exit_4.sysc_n = &wire_3_0_4_0;

    // Wiring the Migen to Migen nodes

    // Wiring the Python to Migen nodes

    // Wiring the Migen to Python nodes

    // Wiring the Python to template nodes

    // Wiring the template to Python nodes

    // Wiring the Migen to template nodes

    // Wiring the template to Migen nodes

    // Add tracing
    wire_0_0_2_0.trace(Tf);
    wire_1_0_2_1.trace(Tf);
    wire_2_0_3_0.trace(Tf);
    wire_3_0_4_0.trace(Tf);







}

void rstprop() {
    // Propogate reset signal to Migen nodes
    rst_bv.write(rst.read());
}

};

#endif
