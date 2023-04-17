

//--------------------------------------------------

#include "../hpp/graphics.hpp"

//--------------------------------------------------


int main (void) {

    Mdl* mdl = mdl_ctor ();


    mdl_work (mdl);


    mdl_dtor (mdl);


    printf ("goodbye!\n");


    return 0;
}

