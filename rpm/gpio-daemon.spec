#
# gpio-daemon spec
# (C) kimmoli 2014
#

Name:       gpio-daemon

# >> macros
# << macros

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}

Summary:    gpio-daemon
Version:    0.1
Release:    1
Group:      Qt/Qt
License:    LICENSE
URL:        https://github.com/kimmoli/gpio-daemon
Source0:    %{name}-%{version}.tar.bz2

BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5DBus)

%description
gpio-daemon

%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
# << build pre

%qtc_qmake5 SPECVERSION=%{version}

%qtc_make %{?_smp_mflags}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
# << install pre

%qmake5_install

# >> install post
# << install post

%files
%defattr(-,root,root,-)
%{_sbindir}/%{name}
/etc/systemd/system/%{name}.service

# >> files
# << files

%post
systemctl start %{name}.service
systemctl enable %{name}.service

%pre
# In case of update, stop first
if [ "$1" = "2" ]; then
  systemctl stop %{name}.service
  systemctl disable %{name}.service
fi

%preun
# in case of complete removal, stop
if [ "$1" = "0" ]; then
  systemctl stop %{name}.service
  systemctl disable %{name}.service
fi

