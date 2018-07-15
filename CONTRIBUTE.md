# Contributing to bspwc

Contributing only involves opening a pull request or an issue. You'll be more
successful with your work if you join us on the [IRC
channel](http://webchat.freenode.net/?channels=bspwc&uio=d4) to discuss your
plan.

## Development cycle

The master branch is a stable point in time. It will always compile on any
given system, but it's labeled as unstable.

All pull requests should be built by the CI successfuly before being merged.

## Pull Requests

If you already have your own pull request habits, feel free to use them. If you
don't, however, allow me to make a suggestion: feature branches pulled from
upstream. Try this:

1. Fork bspwc
2. Clone your fork
3. git remote add upstream git://github.com/Bl4ckb0ne/bspwc.git

You only need to do this once. You're never going to use your fork's master
branch. Instead, when you start working on a feature, do this:

1. git fetch upstream
2. git checkout -b add-so-and-so-feature upstream/master
3. work
4. git push -u origin add-so-and-so-feature
5. Make pull request from your feature branch

## Commit Messages

Be short and concise. The first line of a commit messages shouldn't exceed 50
characters and should be a complete sentance.

The subsequent lines should be seperated from the subject line by a single
blank line, and include optional details. In this you can give justification
for the change, [reference Github
issues](https://help.github.com/articles/closing-issues-via-commit-messages/),
or explain some of the subtler details of your patch. This is important because
when someone finds a line of code they don't understand later, they can use the
`git blame` command to find out what the author was thinking when they wrote
it. It's also easier to review your pull requests if they're seperated into
logical commits that have good commit messages and justify themselves in the
extended commit description.

As a good rule of thumb, anything you might put into the pull request
description on Github is probably fair game for going into the extended commit
message as well.

## Coding Style

bspwc is written in C and follows a style similar to the [kernel
style](https://www.kernel.org/doc/Documentation/process/coding-style.rst),
with a few differences.

Keep your code conforming to C11 and POSIX, and do not use GNU extensions.

### Brackets

Brackets always goes on the next line, also including single statements for `if`
, `while` and `for`.

```c
struct something
{
	char* name;
	struct stuff* stuff;
};

void process_something(const something* something)
{
	if (condition1)
	{
		do_thing_1();
	}
	else
	{
		do_thing_2();
	}
}
```

### Indentation

Indentation is a single tab. Long lines are broken at 80 characters, and
continued beneath with an extra tab. If the line being broken is opening a new
block (functions, if, while, etc.), the continuation line should be indented
with two tabs, so they can't be misread as being part of the block.

```c
void function(void)
{
	if (condition1 && contition2 &&
			condition 3 && condition4)
	{
		really_really_long_function(argument1, argument2,
			argument3, argument4);
	}
}
```

### Naming

bspwc is written in [snake_case](https://en.wikipedia.org/wiki/Snake_case).

### Pointers

The `*` goes next to the type. If you must declare two pointers on the same
line, don't. Do it on two lines and initialize them both to `NULL`. If a pointer
is not allocated or copied on the same line it has been declared, then it must
be initialized to `NULL`.

```c
char* name = NULL;
char* other_name = NULL;

name = get_name();
```

### Construction and destruction functions

Those function are responsible for allocating and freeing an object. They should
be written as a pair.

Syntax is `create_X` and `destroy_X` where `X` is the name of a `struct`.

`create` will allocate the memory for the object, and initialize all its fields.
The function **can return** `NULL` if something went wrong.
`destroy` will free and/or deinitialize all the fields of the object, and free
the object itself.

### Initialize and terminate function

Those function are responsible for initializing and terminating an object. They
should be written as a pair.

Syntax is `initialize_X` and `terminate_X` where `X` is the name of a `struct`.

`initialize` and `terminate` accept a pointer to a given object, and will
initialize or terminate its fields. They are **not** responsible for allocating
or freeing any memory. They are returning a `bool` for the status of the action.

### Error codes

Functions that are not returning a value, nor a pointer should retourn a `bool`
(stdbool.h) if the operation succeeded or not.

### Misc

* Do not use typedefs unless you have a good reason
* Do not use macros unless you have a good reason
* Use `wlr_log` with reckless abandon

