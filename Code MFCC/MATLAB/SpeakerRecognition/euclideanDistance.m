function [ D ] = euclideanDistance( V1, V2 )
%find euclidienne distance between vector V1 and V2

%G = [0.623718635104193	1.45243520566871	0.970756858062032	1.30655224629252	1.12373578611602	1.22714871358943	0.711445291307855	1.31438887935784	1.36313310418481	0.480797128709225	0.955877998983361	0.836979452131063	1.03913345092316];
%G = [1.09213255460442	1.54800018105827	1.38392127621518	1.22681505383073	1.10056489558882	0.865404499758231	0.971196127923510	1.45550001382575	0.494623335484935	0.840755634819192	0.952799193959973	1.03706876048617	0.554029989008967];
%G = [0.195651817180454	2.36750847459184	0.0936781356161534	0.950956303480756	0.754872753850133	0.138136974287625	0.0189687196601914	1.48478722229439	0.0419026591639373	0.00551195038028096	1.84665401711526	1.00127209716060	1.28476291910536];
%G = [0.760745501823928	1.65292199606984	1.08384288533242	1.39246195537652	1.28724723679211	1.06308066960759	1.40602030850354	0.965775391408541	1.16381666681136	0.533866973589834	0.633411177484643	1.22956784759445	0.893769506356881];

%G = [1 0.5 ones(1,11)];
%D = sum(G.*(V1-V2).^2);
D = sum((V1-V2).^2);

end

