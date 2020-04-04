#include <libsystem/eventloop/EventLoop.h>
#include <libsystem/eventloop/Notifier.h>
#include <libsystem/eventloop/Timer.h>
#include <libsystem/io/Socket.h>
#include <libsystem/io/Stream.h>
#include <libsystem/logger.h>
#include <libsystem/process/Launchpad.h>

#include "Compositor/Client.h"
#include "Compositor/Cursor.h"
#include "Compositor/Manager.h"
#include "Compositor/Renderer.h"
#include "Compositor/Window.h"

void keyboard_callback(Notifier *notifier, Stream *keyboard_stream)
{
    __unused(notifier);

    char c;
    stream_read(keyboard_stream, &c, sizeof(char));

    Window *window = manager_focus_window();

    if (window != NULL)
    {
        KeyboardEvent event = (KeyboardEvent){{EVENT_KEYBOARD_KEY_TYPED, false}, c};
        window_send_event(window, (Event *)&event, sizeof(KeyboardEvent));
    }

    if (c == 'q')
    {
        eventloop_exit(0);
    }
}

void mouse_callback(Notifier *notifier, Stream *mouse_stream)
{
    __unused(notifier);

    MousePacket packet;
    size_t size = stream_read(mouse_stream, &packet, sizeof(MousePacket));

    if (size == sizeof(MousePacket))
    {
        cursor_handle_packet(packet);
    }
    else
    {
        logger_warn("Invalid mouse packet!");
    }
}

void accept_callback(Notifier *notifier, Socket *socket)
{
    __unused(notifier);
    __unused(socket);

    Connection *incoming_connection = socket_accept(socket);

    client_create(incoming_connection);
}

void render_callback(Timer *timer)
{
    __unused(timer);

    renderer_repaint_dirty();
}

int main(int argc, char const *argv[])
{
    __unused(argc);
    __unused(argv);

    eventloop_initialize();

    Stream *keyboard_stream = stream_open("/dev/keyboard", OPEN_READ);
    Notifier *keyboard_notifier = notifier_create(HANDLE(keyboard_stream), SELECT_READ);
    keyboard_notifier->on_ready_to_read = (NotifierHandler)keyboard_callback;

    Stream *mouse_stream = stream_open("/dev/mouse", OPEN_READ);
    Notifier *mouse_notifier = notifier_create(HANDLE(mouse_stream), SELECT_READ);
    mouse_notifier->on_ready_to_read = (NotifierHandler)mouse_callback;

    Socket *socket = socket_open("/srv/compositor.ipc", OPEN_CREATE);
    Notifier *socket_notifier = notifier_create(HANDLE(socket), SELECT_ACCEPT);
    socket_notifier->on_ready_to_accept = (NotifierHandler)accept_callback;

    Timer *repaint_timer = timer_create(1000 / 60, render_callback);
    timer_start(repaint_timer);

    manager_initialize();
    cursor_initialize();
    renderer_initialize();

    Launchpad *demoLines = launchpad_create("__demolines", "/bin/__demolines");
    launchpad_launch(demoLines, NULL);

    /*
    Launchpad *demo3D = launchpad_create("__democube", "/bin/__democube");
    launchpad_launch(demo3D, NULL);

    Launchpad *democolors = launchpad_create("__democolors", "/bin/__democolors");
    launchpad_launch(democolors, NULL);

    Launchpad *demogfx = launchpad_create("__demogfx", "/bin/__demogfx");
    launchpad_launch(demogfx, NULL);
    */

    Launchpad *terminal = launchpad_create("Terminal", "/bin/Terminal");
    launchpad_launch(terminal, NULL);

    return eventloop_run();
}
