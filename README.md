# TraceFilter_signalgen
***************************************************************************
  *  Copyright S. V. Paulauskas 2014-2016                                   *
  *                                                                        *
  *  This program is free software: you can redistribute it and/or modify  *
  *  it under the terms of the GNU General Public License as published by  *
  *  the Free Software Foundation, version 3.0 License.                    *
  *                                                                        *
  *  This program is distributed in the hope that it will be useful,       *
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
  *  GNU General Public License for more details.                          *
  *                                                                        *
  *  You should have received a copy of the GNU General Public License     *
  *  along with this program.  If not, see <http://www.gnu.org/licenses/>. *
  **************************************************************************
*/
/** \file test.cpp
 *  \brief A test code to test the filtering
 *  \author S. V. Paulauskas
 *  \date 23 April 2014
 *
 *  This code is based off of the IGOR macro energy.ipf
 *  written by H. Tan of XIA LLC and parts of the nscope
 *  program written at the NSCL written by C.Prokop.
 *
 */
// Adaptation of the above TraceFilter code to accommodate randomised input preamp signals
// Adapted by JJ van Zyl, June 2018
// Compile with:
// g++ signal_trace.cpp TraceFilter.cpp `root-config --libs --cflags --glibs` -o signal_trace
// or
// make -j8
// To run, provide input signal parameters (file: sig_input.dat), as well as trace parameters  (file: trc_params.dat) 
