//------------------------------------------------------------------------------
// Copyright 2018 H2O.ai
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//------------------------------------------------------------------------------
#ifndef dt_FRAME_PYFRAME_h
#define dt_FRAME_PYFRAME_h
#include "python/ext_type.h"
#include "datatable.h"
#include "py_datatable.h"

namespace pydatatable {  // temp
  void _clear_types(obj*);
  PyObject* check(obj*, PyObject*);
}


namespace py {


/**
 * This class currently serves as a base for datatable.Frame, but eventually
 * all functionality will be moved here, and this class will be the main
 * user-facing Frame class.
 */
class Frame : public PyObject {
  private:
    DataTable* dt;
    mutable PyObject* stypes;  // memoized tuple of stypes
    mutable PyObject* ltypes;  // memoized tuple of ltypes

    pydatatable::obj* core_dt;  // TODO: remove

  public:
    class Type : public ExtType<Frame> {
      public:
        static PKArgs args___init__;
        static NoArgs args__repr_html_;
        static PKArgs args_cbind;
        static PKArgs args_colindex;
        static PKArgs args_replace;
        static NoArgs args_copy;
        static NoArgs args_to_dict;
        static NoArgs args_to_list;
        static NoArgs args_to_tuples;
        static const char* classname();
        static const char* classdoc();
        static bool is_subclassable() { return true; }
        static void init_methods_and_getsets(Methods&, GetSetters&);
      private:
        static void _init_names(Methods&, GetSetters&);
        static void _init_init(Methods&, GetSetters&);
    };

    // Internal "constructor" of Frame objects. We do not use real constructors
    // because Frame objects must be allocated/initialized by Python.
    static Frame* from_datatable(DataTable* dt);
    DataTable* get_datatable() const { return dt; }

    void m__init__(PKArgs&);
    void m__dealloc__();
    void m__get_buffer__(Py_buffer* buf, int flags) const;
    void m__release_buffer__(Py_buffer* buf) const;
    oobj m__getitem__(robj item);
    void m__setitem__(robj item, robj value);
    oobj m__getstate__(const NoArgs&);  // pickling support
    void m__setstate__(const PKArgs&);

    oobj _repr_html_(const NoArgs&);
    oobj get_ncols() const;
    oobj get_nrows() const;
    oobj get_shape() const;
    oobj get_stypes() const;
    oobj get_ltypes() const;
    oobj get_names() const;
    oobj get_key() const;
    oobj get_internal() const;
    void set_nrows(robj);
    void set_names(robj);
    void set_key(robj);
    void set_internal(robj);

    void cbind(const PKArgs&);
    oobj colindex(const PKArgs&);
    oobj copy(const NoArgs&);
    void replace(const PKArgs&);
    oobj to_dict(const NoArgs&);
    oobj to_list(const NoArgs&);
    oobj to_tuples(const NoArgs&);
    oobj head(const PKArgs&);
    oobj tail(const PKArgs&);
    void repeat(const PKArgs&);

  private:
    static bool internal_construction;
    class NameProvider;
    void _clear_types() const;
    void _clear_names();
    void _init_names() const;
    void _init_inames() const;
    void _fill_default_names();
    void _dedup_and_save_names(NameProvider*);
    void _replace_names_from_map(py::odict);

    // getitem / setitem support
    oobj _main_getset(robj item, robj value);

    friend void pydatatable::_clear_types(pydatatable::obj*); // temp
    friend PyObject* pydatatable::check(pydatatable::obj*, PyObject*); // temp
    friend class FrameInitializationManager;
    friend class pylistNP;
    friend class strvecNP;
};

extern PyObject* Frame_Type;
extern PyObject* fread_fn;

}  // namespace py

#endif
