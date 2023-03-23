#pragma once

#include <karm-sys/path.h>
#include <karm-ui/react.h>

namespace FileManager {

struct State {
    Vec<Sys::Path> history;
    usize currentIndex = 0;

    State(Sys::Path path)
        : history({path}) {}

    Sys::Path currentPath() const {
        return history[currentIndex];
    }

    bool canGoBack() const {
        return currentIndex > 0;
    }

    bool canGoForward() const {
        return currentIndex + 1 < history.len();
    }

    bool canGoParent() const {
        return currentPath().hasParent();
    }
};

struct GoRoot {};

struct GoHome {};

struct GoBack {};

struct GoForward {};

struct GoParent {
    isize index;
};

struct GoTo {
    Sys::Path path;
};

struct Refresh {};

struct AddBookmark {};

using Actions = Var<GoRoot, GoHome, GoBack, GoForward, GoParent, GoTo, Refresh, AddBookmark>;

State reduce(State d, Actions action);

using Model = Ui::Model<State, Actions, reduce>;

} // namespace FileManager
