// Part of Arac Neural Network Composition Library.
// (c) 2008 by Justin S Bayer, <bayer.justin@googlemail.com>


#ifndef Arac_STRUCTURE_COMPONENT_INCLUDED
#define Arac_STRUCTURE_COMPONENT_INCLUDED


#include <cassert>


namespace arac {
namespace structure {
    

class Component 
{
    public: 
        
        enum Mode 
        {
            Simple = 0,
            ErrorAgnostic = 1,
            Sequential = 2,
            
            SequentialErrorAgnostic = 3,
        };
        
        Component();
        
        virtual ~Component();
        
        virtual void forward();
        virtual void backward();
        
        virtual void dry_forward();
        virtual void dry_backward();
        
        // Set the mode of the module.
        virtual void set_mode(Mode mode);
        
        // Get the mode of the module.
        Mode get_mode();
        
        // Tell if the module is sequential.
        bool sequential();
        
        // Return the current timestep.
        int timestep();

        // Return the sequence length of the current sequence.
        int sequencelength();
        
        // Tell if the module is error agnostic.
        bool error_agnostic();
        
        
    protected:

        virtual void pre_forward();
        virtual void pre_backward();

        virtual void post_forward();
        virtual void post_backward();

        virtual void _forward() = 0;
        virtual void _backward() = 0;
        
        int _timestep;
        int _sequencelength;
        Mode _mode;
        
};


inline Component::Component() : 
    _timestep(0),
    _sequencelength(0),
    _mode(Component::Simple)
{
}

inline Component::~Component() {}


inline
void
Component::forward()
{
    pre_forward();
    _forward();
    post_forward();
}


inline
void 
Component::backward()
{
    pre_backward();
    _backward();
    post_backward();
}


inline
void
Component::dry_forward()
{
    pre_forward();
    post_forward();
}


inline void
Component::dry_backward()
{
    pre_backward();
    post_backward();
}


inline
void
Component::pre_forward()
{
    if (!sequential())
    {
        _timestep = 0;
    }
}


inline
void
Component::pre_backward()
{
    if (!sequential())
    {
        _timestep = 1;
    }
}


inline
void
Component::post_forward()
{
    if (sequential())
    {
        _timestep += 1;
        _sequencelength += 1;
    }
    else
    {
        _timestep = 1;
        _sequencelength = 1;
    }
}


inline
void
Component::post_backward()
{
    if (sequential())
    {
        _timestep -= 1;
    }
    else
    {
        _timestep = 0;
    }
}


inline 
Component::Mode
Component::get_mode()
{
    return _mode;
}


inline 
void
Component::set_mode(Component::Mode mode)
{
    _mode = mode;
}


inline
bool
Component::sequential()
{
    return _mode & Component::Sequential;
}


inline
int
Component::sequencelength()
{
    return _sequencelength;
}


inline
bool 
Component::error_agnostic()
{
    return _mode & Component::ErrorAgnostic;
}


inline
int
Component::timestep()
{
    return _timestep;
}


}
}


#endif