/*****************************************************************************
*
* Copyright (c) 2000 - 2012, Lawrence Livermore National Security, LLC
* Produced at the Lawrence Livermore National Laboratory
* LLNL-CODE-442911
* All rights reserved.
*
* This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
* full copyright notice is contained in the file COPYRIGHT located at the root
* of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
*
* Redistribution  and  use  in  source  and  binary  forms,  with  or  without
* modification, are permitted provided that the following conditions are met:
*
*  - Redistributions of  source code must  retain the above  copyright notice,
*    this list of conditions and the disclaimer below.
*  - Redistributions in binary form must reproduce the above copyright notice,
*    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
*    documentation and/or other materials provided with the distribution.
*  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
*    be used to endorse or promote products derived from this software without
*    specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
* ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
* LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
* DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
* SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
* CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
* LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
* OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*
*****************************************************************************/

// ************************************************************************* //
//                              avtIVPAdamsBashforth.C                       //
// ************************************************************************* //

#include <avtIVPAdamsBashforth.h>
#include <avtIVPStateHelper.h>
#include <DebugStream.h>

#include <limits>
#include <cmath>

static const double epsilon = std::numeric_limits<double>::epsilon();

// constants for the A-B scheme.
static const double bashforth[] = { 1901.0, -2774.0, 2616.0, -1274.0, 251.0 };
static const double divisor = 1.0 / 720.0;

// helper function
// returns a with the same sign as b
static inline double sign( const double& a, const double& b )
{
    return (b > 0.0) ? std::abs(a) : -std::abs(a);
}

// ****************************************************************************
//  Method: avtIVPAdamsBashforth constructor
//
//  Programmer: Dave Pugmire
//  Creation:   August 5, 2008
//
//  Modifications:
//    Dave Pugmire, Fri Aug  8 16:05:34 EDT 2008
//    Improved version of A-B solver that builds function history from
//    initial RK4 steps.
//
//    Dave Pugmire, Tue Aug 19, 17:38:03 EDT 2008
//    Changed how distanced based termination is computed.
//
//    Dave Pugmire, Wed Aug 20, 12:54:44 EDT 2008
//    Add a tolerance and counter for handling stiffness detection.
//
//    Dave Pugmire, Tue Feb 24 14:35:38 EST 2009
//    Remove moulton corrector code, use RK4 at startup, terminate on numSteps.
//
// ****************************************************************************

avtIVPAdamsBashforth::avtIVPAdamsBashforth()
{
    // set (somewhat) reasonable defaults
    tol = 1e-8;
    h = 1e-5;
    t = 0.0;
    d = 0.0;
    numStep = 0;
    degenerate_iterations = 0;
    stiffness_eps = tol / 1000.0;
}

// ****************************************************************************
//  Method: avtIVPAdamsBashforth destructor
//
//  Programmer: Dave Pugmire
//  Creation:   August 5, 2008
//
// ****************************************************************************


avtIVPAdamsBashforth::~avtIVPAdamsBashforth()
{
}


// ****************************************************************************
//  Method: avtIVPAdamsBashforth::GetCurrentT
//
//  Purpose:
//      Gets the current T.
//
//  Programmer: Dave Pugmire
//  Creation:   August 5, 2008
//
// ****************************************************************************

double 
avtIVPAdamsBashforth::GetCurrentT() const 
{
    return t;
}


// ****************************************************************************
//  Method: avtIVPAdamsBashforth::GetCurrentY
//
//  Purpose:
//      Gets the current Y.
//
//  Programmer: Dave Pugmire
//  Creation:   August 5, 2008
//
//  Modifications:
//    Dave Pugmire, Fri Aug  8 16:05:34 EDT 2008
//    Improved version of A-B solver that builds function history from
//    initial RK4 steps.
//
//    Dave Pugmire, Tue Dec  1 11:50:18 EST 2009
//    Switch from avtVec to avtVector.
//
// ****************************************************************************

avtVector 
avtIVPAdamsBashforth::GetCurrentY() const
{
    return yCur;
}

// ****************************************************************************
//  Method: avtIVPAdamsBashforth::SetCurrentY
//
//  Purpose:
//      Sets the current Y.
//
//  Programmer: Dave Pugmire
//  Creation:   August 5, 2008
//
//  Modifications:
//    Dave Pugmire, Fri Aug  8 16:05:34 EDT 2008
//    Improved version of A-B solver that builds function history from
//    initial RK4 steps.
//
//    Dave Pugmire, Tue Dec  1 11:50:18 EST 2009
//    Switch from avtVec to avtVector.
//
// ****************************************************************************

void
avtIVPAdamsBashforth::SetCurrentY(const avtVector &newY)
{
    yCur = newY;
}


// ****************************************************************************
//  Method: avtIVPAdamsBashforth::SetCurrentT
//
//  Purpose:
//      Sets the current T.
//
//  Programmer: Dave Pugmire
//  Creation:   August 5, 2008
//
// ****************************************************************************

void
avtIVPAdamsBashforth::SetCurrentT(double newT)
{
    t = newT;
}


// ****************************************************************************
//  Method: avtIVPAdamsBashforth::SetNextStepSize
//
//  Purpose:
//      Sets the step size for the next step.
//
//  Programmer: Dave Pugmire
//  Creation:   August 5, 2008
//
// ****************************************************************************

void 
avtIVPAdamsBashforth::SetNextStepSize(const double& _h)
{
    h = _h;
}


// ****************************************************************************
//  Method: avtIVPAdamsBashforth::GetNextStepSize
//
//  Purpose:
//      Gets the step size for the next step.
//
//  Programmer: Dave Pugmire
//  Creation:   August 5, 2008
//
// ****************************************************************************

double 
avtIVPAdamsBashforth::GetNextStepSize() const
{
    return h;
}


// ****************************************************************************
//  Method: avtIVPAdamsBashforth::SetMaximumStepSize
//
//  Purpose:
//      Sets the maximum step size for the next step.
//
//  Programmer: Dave Pugmire
//  Creation:   August 5, 2008
//
// ****************************************************************************

void
avtIVPAdamsBashforth::SetMaximumStepSize(const double& hMax)
{
    h_max = hMax;
}


// ****************************************************************************
//  Method: avtIVPAdamsBashforth::SetTolerances
//
//  Purpose:
//      Sets the tolerance.
//
//  Programmer: Dave Pugmire
//  Creation:   August 5, 2008
//
//  Modifications:
//    Dave Pugmire, Wed Aug 20, 12:54:44 EDT 2008
//    Add a tolerance and counter for handling stiffness detection.
//
// ****************************************************************************

void
avtIVPAdamsBashforth::SetTolerances(const double& relt, const double& abst)
{
    tol = abst;
    stiffness_eps = tol / 1000.0;
}

// ****************************************************************************
//  Method: avtIVPAdamsBashforth::Reset
//
//  Purpose:
//      Resets data members.  Called by the constructors.
//
//  Programmer: Dave Pugmire
//  Creation:   August 5, 2008
//
//  Modifications:
//    Dave Pugmire, Fri Aug  8 16:05:34 EDT 2008
//    Improved version of A-B solver that builds function history from
//    initial RK4 steps.
//
//    Dave Pugmire, Tue Aug 19, 17:38:03 EDT 2008
//    Changed how distanced based termination is computed.
//
//    Dave Pugmire, Wed Aug 20, 12:54:44 EDT 2008
//    Add a tolerance and counter for handling stiffness detection.
//
//    Dave Pugmire, Mon Feb 23, 09:11:34 EST 2009
//    Reworked the termination code. Added a type enum and value. Made num steps
//    a termination criterion. Code cleanup: We no longer need fwd/bwd solvers.
//    Removed the plane intersection code.
//
//    Dave Pugmire, Tue Mar 10 12:41:11 EDT 2009
//    Bug fix in parallel communication of solver state.
//
//    Dave Pugmire, Tue May  5 10:43:05 EDT 2009
//    Memory issue with history init. Make sure vecs are of proper size.
//
//    Dave Pugmire, Tue Dec  1 11:50:18 EST 2009
//    Switch from avtVec to avtVector.
//
// ****************************************************************************

void 
avtIVPAdamsBashforth::Reset(const double& t_start,
                            const avtVector &y_start,
                            const avtVector &v_start)
{
    t = t_start;
    d = 0.0;
    numStep = 0;

    degenerate_iterations = 0;
    yCur = y_start;
    h = h_max;
}


// ****************************************************************************
//  Method: avtIVPAdamsBashforth::OnExitDomain
//
//  Purpose:
//      Post processing tasks after domain exit.
//
//  Programmer: Dave Pugmire
//  Creation:   August 5, 2008
//
//  Modifications:
//    Dave Pugmire, Fri Aug  8 16:05:34 EDT 2008
//    Improved version of A-B solver that builds function history from
//    initial RK4 steps.
//
//    Dave Pugmire, Tue Mar 10 12:41:11 EDT 2009
//    Bug fix in parallel communication of solver state.
//
// ****************************************************************************

void
avtIVPAdamsBashforth::OnExitDomain()
{
}


// ****************************************************************************
//  Method: avtIVPAdamsBashforth::RK4Step
//
//  Purpose:
//      Take a step and return the result.
//
//  Programmer: Dave Pugmire
//  Creation:   Feb 24 2009
//
//  Modifications:
//
//    Dave Pugmire, Tue Dec  1 11:50:18 EST 2009
//    Switch from avtVec to avtVector.
//
//    Hank Childs, Mon Mar 12 10:26:33 PDT 2012
//    Integrate fix from Christoph Garth.
//
// ****************************************************************************

avtIVPSolver::Result 
avtIVPAdamsBashforth::RK4Step(const avtIVPField* field,
                              avtVector &yNew )
{
    avtVector f[4];
    avtIVPField::Result result;
    
    if ((result = (*field)(t, yCur,              f[0])) != avtIVPField::OK)
        return ConvertResult(result);
    
    f[0] *= h;
    if ((result = (*field)(t+0.5*h, yCur + f[0] * 0.5, f[1])) != avtIVPField::OK)
        return ConvertResult(result);
    
    f[1] *= h;
    if ((result = (*field)(t+0.5*h, yCur + f[1] * 0.5, f[2])) != avtIVPField::OK)
        return ConvertResult(result);
    
    f[2] *= h;
    if ((result = (*field)(t+h, yCur + f[2],       f[3])) != avtIVPField::OK)
        return ConvertResult(result);
    
    f[3] *= h;
    yNew = yCur + (f[0] + 2.0 * f[1] + 2.0 * f[2] + f[3]) * (1.0 / 6.0);
    
    return avtIVPSolver::OK;
}


// ****************************************************************************
//  Method: avtIVPAdamsBashforth::ABStep
//
//  Purpose:
//      Take a step and return the result.
//
//  Programmer: Dave Pugmire
//  Creation:   August 5, 2008
//
//  Modifications:
//
//    Dave Pugmire, Tue Feb 24 14:35:38 EST 2009
//    Remove moulton corrector code, use RK4 at startup, terminate on numSteps.
//
//    Dave Pugmire, Tue Mar 10 12:41:11 EDT 2009
//    Bug fix in parallel communication of solver state.
//
// ****************************************************************************

avtIVPSolver::Result 
avtIVPAdamsBashforth::ABStep(const avtIVPField* field,
                             avtVector &yNew )
{
    // Calculate the predictor using the Adams-Bashforth formula
    yNew = yCur;

    for (size_t i = 0; i < ADAMS_BASHFORTH_NSTEPS; i++)
        yNew += h*divisor*bashforth[i] * history[i];

    return avtIVPSolver::OK;
}

// ****************************************************************************
//  Method: avtIVPAdamsBashforth::Step
//
//  Purpose:
//      Take a step and return the result.
//
//  Programmer: Dave Pugmire
//  Creation:   August 5, 2008
//
//  Modifications:
//
//    Dave Pugmire, Wed Aug 13 10:58:32 EDT 2008
//    Store the velocity with each step.
//
//    Dave Pugmire, Tue Aug 19, 17:38:03 EDT 2008
//    Chagned how distanced based termination is computed.
//
//    Dave Pugmire, Wed Aug 20, 12:54:44 EDT 2008
//    Add a tolerance and counter for handling stiffness detection.
//
//    Dave Pugmire, Mon Feb 23, 09:11:34 EST 2009
//    Reworked the termination code. Added a type enum and value. Made num steps
//    a termination criterion. Code cleanup: We no longer need fwd/bwd solvers.
//    Removed the plane intersection code.
//
//    Dave Pugmire, Tue Feb 24 14:35:38 EST 2009
//    Remove moulton corrector code, use RK4 at startup, terminate on numSteps.
//
//    Dave Pugmire, Tue Mar 10 12:41:11 EDT 2009
//    Bug fix in parallel communication of solver state.
//
//   Dave Pugmire, Tue Aug 11 10:25:45 EDT 2009
//   Add new termination criterion: Number of intersections with an object.
//
//   Dave Pugmire, Tue Feb 23 09:42:25 EST 2010
//   Set the velStart/velEnd direction based on integration direction.
//
//   Dave Pugmire, Wed May 26 13:48:24 EDT 2010
//   The velStart/velEnd direction was reversed.
//
// ****************************************************************************

avtIVPSolver::Result 
avtIVPAdamsBashforth::Step(avtIVPField* field, double t_max,
                           avtIVPStep* ivpstep)
{
    const double direction = sign( 1.0, t_max - t );
    
    h = sign( h, direction );
    
    bool last = false;

    // do not run past integration end
    if( (t + 1.01*h - t_max) * direction > 0.0 ) 
    {
        last = true;
        h = t_max - t;
    }

    // stepsize underflow?
    if( 0.1*std::abs(h) <= std::abs(t)*epsilon )
        return avtIVPSolver::STEPSIZE_UNDERFLOW;

    avtIVPSolver::Result res;
    avtIVPField::Result fieldResult;
    avtVector yNew = yCur;

    // Use a fourth-order Runga Kutta integration to seed the Adams-Bashforth.
    if( numStep < ADAMS_BASHFORTH_NSTEPS )
    {
        // Save the first vector values in the history. 
        if( numStep == 0 )
        {
            if ((fieldResult = (*field)(t, yCur, history[0])) != avtIVPField::OK)
                return ConvertResult(fieldResult);
        }
         
        res = RK4Step( field, yNew );
    }
    else
        res = ABStep( field, yNew );

    if (res == OK)
    {
        ivpstep->resize(2);

        if( convertToCartesian )
        {
          (*ivpstep)[0] = field->ConvertToCartesian( yCur );
          (*ivpstep)[1] = field->ConvertToCartesian( yNew );
        }
        else
        {
          (*ivpstep)[0] = yCur;
          (*ivpstep)[1] = yNew;
        }

        ivpstep->t0 = t;
        ivpstep->t1 = t + h;
        numStep++;

        // Update the history to save the last 5 vector values.
        history[4] = history[3];
        history[3] = history[2];
        history[2] = history[1];
        history[1] = history[0];
        if ((fieldResult = (*field)(t, yNew, history[0])) != avtIVPField::OK)
            return ConvertResult(fieldResult);

        yCur = yNew;
        t = t+h;

        if( last )
            res = avtIVPSolver::TERMINATE;

        // Reset the step size on sucessful step.
        h = h_max;
    }

    return res;
}

// ****************************************************************************
//  Method: avtIVPAdamsBashforth::AcceptStateVisitor
//
//  Purpose:
//      Loads the state into the state helper.
//
//  Programmer: Dave Pugmire
//  Creation:   August 5, 2008
//
//  Modifications:
//
//    Dave Pugmire, Wed Aug 20, 12:54:44 EDT 2008
//    Add a tolerance and counter for handling stiffness detection.
//
//    Dave Pugmire, Tue Mar 10 12:41:11 EDT 2009
//    Bug fix in parallel communication of solver state.
//
// ****************************************************************************
void
avtIVPAdamsBashforth::AcceptStateVisitor(avtIVPStateHelper& aiss)
{
    aiss.Accept(numStep)
        .Accept(tol)
        .Accept(degenerate_iterations)
        .Accept(stiffness_eps)
        .Accept(h)
        .Accept(h_max)
        .Accept(t)
        .Accept(d)
        .Accept(yCur)
        .Accept(history[0])
        .Accept(history[1])
        .Accept(history[2])
        .Accept(history[3])
        .Accept(history[4]);
}
