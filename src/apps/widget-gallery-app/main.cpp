#include <karm-main/main.h>
#include <karm-ui/app.h>
#include <karm-ui/dialog.h>
#include <karm-ui/input.h>
#include <karm-ui/layout.h>
#include <karm-ui/scafold.h>
#include <karm-ui/scroll.h>
#include <karm-ui/view.h>

void willShowMessage(Ui::Node &n) {
    Ui::showMsgDialog(n, "Button pressed");
}

Ui::Child buttons(bool enable) {
    return Ui::hflow(
        8,
        Ui::button(enable ? willShowMessage : Ui::OnPress{}, Ui::ButtonStyle::primary(), "PRIMARY BUTTON"),
        Ui::button(enable ? willShowMessage : Ui::OnPress{}, "BUTTON"),
        Ui::button(enable ? willShowMessage : Ui::OnPress{}, Ui::ButtonStyle::outline(), "OUTLINE BUTTON"),
        Ui::button(enable ? willShowMessage : Ui::OnPress{}, Ui::ButtonStyle::subtle(), "SUBTLE BUTTON"),
        Ui::button(enable ? willShowMessage : Ui::OnPress{}, Ui::ButtonStyle::destructive(), "DESTRUCTIVE BUTTON"));
}

Ui::Child badges() {
    return Ui::hflow(
        8,
        Ui::badge(Ui::BadgeStyle::ERROR, "Error"),
        Ui::badge(Ui::BadgeStyle::WARNING, "Warning"),
        Ui::badge(Ui::BadgeStyle::SUCCESS, "Success"),
        Ui::badge(Ui::BadgeStyle::INFO, "Info"));
}

Ui::Child texts() {
    return Ui::vflow(
        8,
        Ui::text(Ui::TextStyle::title1(), "The quick brown fox jumps over the lazy dog."),
        Ui::text(Ui::TextStyle::title2(), "The quick brown fox jumps over the lazy dog."),
        Ui::text(Ui::TextStyle::title3(), "The quick brown fox jumps over the lazy dog."),
        Ui::text(Ui::TextStyle::subtitle1(), "The quick brown fox jumps over the lazy dog."),
        Ui::text(Ui::TextStyle::subtitle2(), "The quick brown fox jumps over the lazy dog."),
        Ui::text(Ui::TextStyle::body(), "The quick brown fox jumps over the lazy dog."));
}

CliResult entryPoint(CliArgs args) {
    auto titlebar = Ui::titlebar(Media::Icons::DUCK, "Widget Gallery");
    auto content = Ui::spacing(
        8,
        Ui::vflow(
            8,
            buttons([](Ui::Node &n) {
                Ui::showMsgDialog(n, "Button pressed");
            }),
            buttons(NONE),
            badges(),
            texts()));

    auto layout = Ui::dialogLayer(
        Ui ::minSize(
            {700, 500},
            Ui::vflow(titlebar,
                      Ui::grow(
                          Ui::vscroll(content)))));

    return Ui::runApp(args, layout);
}