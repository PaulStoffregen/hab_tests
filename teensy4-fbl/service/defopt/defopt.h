#ifndef DEFOPT_H
#define DEFOPT_H

/*
 * Options string syntax:
 *
 *  <opt-char>[:[:]][,<opt-char>[:[:]]*
 *       |     | |
 *       |     | Option takes optional argument
 *       |     Option takes mandatory argument
 *       Character identifying the option
 */

/*
 * Long options structure syntax:
 *
 *  <opt-string>, <has-arg>, <flags>, <...>
 *       |            |         |       |
 *       |            |         |
 *       |            |         pointer to intager which is returned if no ... is given 
 *       |            Identifies whether and what kind of argument the option takes:
 *       |              0..no_argument, 1..required_argument, 2..optional_argument
 */

#define ARG0
#define ARG1 ,':'
#define ARG2 ,':',':'

#define _CAT(a, b) a ## b
#define CAT(a, b) _CAT(a, b)

#define _MKSTR(a) # a
#define MKSTR(a) _MKSTR(a)

#define _MKOPT(opt, arg) opt CAT(ARG, arg)
#define MKOPT(opt) _MKOPT(CAT(OPT_, opt), CAT(OPT_, CAT(opt, _ARGS)))
#define LASTOPT() 0

#define _MKLOPT(lopt, arg, id) {lopt, arg, 0, id}
#define MKLOPT(opt) _MKLOPT(CAT(OPT_, CAT(opt, _LONG)), CAT(OPT_, CAT(opt, _ARGS)), CAT(OPT_, opt))
#define LASTLOPT() {0, 0, 0, 0}

#define _LONGOPT(opt) opt + 256
#define LONGOPT(opt) _LONGOPT(opt)

#define _MKDESC(opt) CAT(OPT_, CAT(opt, _DESC))
#define MKDESC(opt) _MKDESC(opt)

#define OPT_TEST 't', "test", 1

#define _DECODE(optID, loptID, args) ()
#define DECODE(field) _DECODE(field)
#endif /* DEFOPT_H */
