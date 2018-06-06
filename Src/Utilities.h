//
// Created by Vladimir on 01.06.2018.
//

#ifndef SOCKS5_UTILITIES_H
#define SOCKS5_UTILITIES_H

#include <array>


namespace Utilities {

enum class ProxyType {
    TCP,
    UDP
};

enum class AuthenticationType {
    NoAuthentication,
    GSSAPI,
    UsernamePassword
};

enum class AddressType {
    IPv4,
    IPv6
};

enum class AuthenticationResponse {
    AccessGranted,
    ProxyError,
    ConnectionProhibited,
    NetworkUnavailable,
    HostUnavailable,
    ConnectionRefused,
    TimeLimitExceeded,
    CommandIsUnsupported,
    AddressTypeIsUnsupported
};

template <size_t BufferSize>
inline size_t MakeConnectionRequest(std::array<char, BufferSize>* buffer) {
    (*buffer)[0] = 0x05;
    (*buffer)[1] = 0x01;
    (*buffer)[2] = 0x00;
    return 3;
}

template <size_t BufferSize>
inline size_t MakeConnectionRequestResponse(std::array<char, BufferSize>* buffer) {
    (*buffer)[0] = 0x05;
    (*buffer)[1] = 0x00;
    return 2;
}
//void MakeAuthenticationRequest();

} // namespace Utilities

#endif //SOCKS5_UTILITIES_H
