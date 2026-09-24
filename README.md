# [M1 DIPAQ] Introduction to Parallel Algorithms and Programming

**Instructor:** Oguz Kaya (oguz.kaya[at]universite-paris-saclay.fr), Université Paris-Saclay and LISN

This course is a practical introduction to parallel computing from two complementary
angles: **parallel programming** on real hardware (SIMD units, multicore CPUs, GPUs,
clusters) and **parallel algorithms** in the PRAM model and on communication networks.

The whole course rests on one principle: *processing unit i executes the same program on
index i*. PRAM algorithms are written exactly in this model, and the four programming
sessions are four ways of running them on real hardware, with progressively fewer
constraints: SIMD lanes in lockstep, OpenMP threads sharing memory, CUDA threads on a GPU,
MPI processes exchanging messages.

The course focuses on principles and algorithmic thinking. A deeper treatment of each
programming paradigm is left to the follow-up courses **High-Performance Computing on
Multicore Architectures**, **MPI Programming on Distributed Memory Architectures**, and
**GPU Programming**.

## Course organization

We will have seven sessions of 3 hours each. Programming sessions combine a lecture
(approximately 1 hour) and a programming lab, TP (approximately 2 hours); algorithmic
sessions combine a lecture and a tutorial, TD, with exercises done on paper. Mini-exams
take place at the end of sessions 4 to 7 (cf. below). Please bring your laptop to every
session; the labs are done in the browser on Compiler Explorer, so no software
installation is needed.

- **Session 1:** Introduction to parallel computing; SIMD programming with AVX intrinsics (TP1)
- **Session 2:** Parallel algorithms: the PRAM model, part 1
- **Session 3:** Multicore programming with OpenMP (TP2)
- **Session 4:** Parallel algorithms: the PRAM model, part 2 (TD1, + mini-exam 1)
- **Session 5:** GPU programming with CUDA (TP3, + mini-exam 2)
- **Session 6:** Parallel algorithms on communication networks (TD2, + mini-exam 3)
- **Session 7:** Distributed memory programming with MPI (TP4, + mini-exam 4)

## Evaluation

### Graded lab assignments

Each lab must be submitted by e-mail to `oguz.kaya[at]universite-paris-saclay.fr` by
**23:59 on the Wednesday following the lab session**, with the e-mail subject

```
M1DIPAQALGOPAR LABX SURNAME(s) Name(s)
```

(replace X by the lab number). Please follow this format for the e-mail subject
**exactly to the letter**; it is what lets me sort the submissions automatically.

- Attach only the source code files (`.cpp`, `.cu`), **one file per exercise**, named as
  in the skeleton (e.g. `copy.cpp`, `dot.cpp` for lab 1), directly attached to the
  e-mail: no zip archive, no folder.
- Answers to the questions of the assignment (timings, speedups, explanations) go as
  comments at the end of the corresponding source file.
- **One** of the four lab assignments will be selected at random and graded, giving up
  to **1/20** points. Submit all of them: you do not know which one will be graded.
- If you do not follow the e-mail subject format in at least one of your submissions,
  you will lose half a point (0.5/20).

### Tutorial (TD) assignments

Nothing is to be sent for the tutorial sessions of the algorithmic weeks. However, some
exercises will be left to you to finish at home; doing them is the best practice for the
mini-exams.

### Solutions

Solutions to labs and tutorials are posted on Thursday, before the class on Monday morning. You are
expected to compare them with your own work beforehand and to come to class with your
questions, if any, for when we discuss them.

### End-of-session mini-exams

At the end of sessions 4, 5, 6 and 7 there will be a written mini-exam of 20-25
minutes, giving **4.75/20** points each (19/20 in total). Mini-exams take place outside
the course hours: the course runs from 9:00 to 12:00, and the mini-exam from 12:00 to
12:30. Each mini-exam covers the
material of all sessions up to that week's session, excluding that week's session: for
instance, mini-exam 1 (session 4) covers the material of sessions 1 to 3, and mini-exam
2 (session 5) covers sessions 1 to 4. Questions are of two kinds.

**Programming questions** (SIMD, OpenMP, CUDA, MPI), of the following types:

- *Debugging:* given a complete code with multiple bugs, identify each bug.
- *Output:* given a complete code, find out what the program could potentially display.
- *Coding:* a skeleton code or a function signature is given for a specific task; write
  the code.
- *Course concepts:* small questions requiring interpretation of the concepts learned.
- *Code deciphering:* given a complete parallel code (with anonymized variable names),
  understand and explain what it computes.

**Algorithmic questions** (PRAM and communication networks), in the style of what we do
in class: a parallel algorithm is given (or you are asked to design one for a given
problem), and you analyze it: parallel time, work, number of processors, speedup and
efficiency, cost optimality, and how it compares to the sequential algorithm.

You can bring four A4 sheets to the mini-exam (both sides, with whatever you want on
them); no other material is allowed.

## Technical tips

We will use [Compiler Explorer](https://godbolt.org) throughout the course to compile
and run the lab codes directly in the browser (with "Execute the code" ticked in the
"Output..." menu of the compiler pane); the GPU session uses its CUDA variant at
[cuda.godbolt.org](https://cuda.godbolt.org). Save your code locally; the site does not
keep it.
