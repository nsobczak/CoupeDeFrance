%-------------Calcule fonction transfert du systeme desiré-----------------

amortissement = 0.7;% coef connue pour sa marche bien
f = 50; % frequence d'echantillonnage
W0 = 2*pi*f;
Wn = 1/10*W0% calcule pulsation naturelle
HDesire = tf(Wn*Wn,[1 2*amortissement*Wn Wn*Wn])% calcule la fonction de transfert desirée

%--------------------------------------------------------------------------

pDesire = pole(HDesire)% calcule les poles de la fonction transfert désires

%--------------------------------------------------------------------------

% -------------Calcule fonction transfert en s du Moteur----------------

K = -0.055;% gain moteur
tau = 0.500;% constante de temps
HMoteurs = tf(K,[tau 1])% fonction transfert moteur

% -------------------------------------------------------------------------

%---------------Calcule fonction transfert en z^-1 du Moteur------------------

alpha1 = -exp(-1/(f*tau))
beta1 = K*(1+alpha1)
HMoteurZ = tf([0 beta1],[1 alpha1],1/f,'variable','z^-1')

%--------------------------------------------------------------------------

%------------Calcule fonction transfert en z^-1 du PID---------------------


r0 = 1; 
r1 = 1;
HPidZ = tf([r0 r1],[1 -1],1/f,'variable','z^-1')

%--------------------------------------------------------------------------

HsystemeZ = tf(HMoteurZ,HPidZ)
