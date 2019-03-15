Name:       harbour-googlephotoshareplugin

# >> macros
# << macros

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}

Summary:    GooglePhoto transfe engine plugin for Sailfish
Version:    0.0.1
Release:    1
Group:      Applications/System
License:    The MIT License (MIT)
URL:        https://github.com/Maledictus/harbour-getpocketshareplugin
Source0:    %{name}-%{version}.tar.bz2

Requires:   sailfishsilica-qt5 >= 0.10.9
Requires:   nemo-transferengine-qt5 >= 0.3.1
Requires:   declarative-transferengine-qt5 >= 0.0.44
Requires:   libsignon-qt5
Requires:   libsailfishkeyprovider
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(nemotransferengine-qt5)
BuildRequires:  pkgconfig(libsignon-qt5)
BuildRequires:  pkgconfig(libsailfishkeyprovider)

%description
Transfer engine plugin gives possibility to share images with your google photo account


%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
# << build pre

%qtc_qmake5

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
%{_libdir}/nemo-transferengine/plugins/*.so
%{_datadir}/nemo-transferengine/plugins/GooglePhotoShareUi.qml
%{_datadir}/%{name}
# >> files
# << files
