function [] = main()
    folder = 'Images';
    %% addpath Core
    %% Initialize the correspondences

    % The initialization is based on NRDC by Y. HaCohen et al.(2011-ACM TOG)
    InitializeM('2.jpg', '1.jpg', folder);
    InitializeM('2.jpg', '3.jpg', folder);

    %% Iteratively match and fusion
    disp('Iterating ...');
    system(['IterMF.exe ' , folder]);
end

%%%%% == Initialization === 
function [] = InitializeM( Src_path, Ref_path , folder)

% The initialization is based on NRDC by Y. HaCohen et al.(2011-ACM TOG)

    %% Read Images
    disp('Loading Images and Initialing ...');
    Src  = double(imread([folder '\' Src_path])) / 255.0;
    Ref  = double(imread([folder '\' Ref_path])) / 255.0;

    %% Initialize
    Para = [];
    [InitCorrs, CorrCosts, ColorT, TempCorr] = Initial(Src, Ref, Para);

    %% Save the result:
    disp('Saving the Initialization ...');
    if isempty(InitCorrs)
        disp('No matching has been found.')
    else
        [dcf_result, conf] = mc(InitCorrs, 8, CorrCosts);
        outimg = cat(3, dcf_result, conf);
        dlmwrite( [folder '\' Src_path, Ref_path, '_1.rel'], outimg(:,:,1), 'delimiter', '\t','precision', 16)
        dlmwrite( [folder '\' Src_path, Ref_path, '_2.rel'], outimg(:,:,2), 'delimiter', '\t','precision', 16)
        dlmwrite( [folder '\' Src_path, Ref_path, '_3.rel'], outimg(:,:,3), 'delimiter', '\t','precision', 16)
        xx = 0:1/10000:1;
        yyC1 = ppval(xx,ColorT.RGBCurves(1));
        yyC2 = ppval(xx,ColorT.RGBCurves(2));
        yyC3 = ppval(xx,ColorT.RGBCurves(3));
        dlmwrite( [folder '\' Src_path, Ref_path, '_ct.rel'], [10000, yyC1, yyC2, yyC3], 'delimiter', '\t','precision', 16)
    end

end

%% 
function [Corr, Corr_Confidence] = mc(DCF, patch_w, DCF_Confidence)

    Corr=zeros([size(DCF,1)+patch_w-1, size(DCF,2)+patch_w-1, 2]);
    Corr(patch_w/2+1:end-patch_w/2+1, patch_w/2+1:end-patch_w/2+1, :) = DCF(:,:,1:2) + patch_w/2;
    
    Corr_Confidence = zeros(size(Corr(:,:,1)));
    if exist('DCF_Confidence', 'var') && ~isempty(DCF_Confidence)
        Corr_Confidence(patch_w/2+1:end-patch_w/2+1, patch_w/2+1:end-patch_w/2+1) = DCF_Confidence(:,:);
    else
        Corr_Confidence(patch_w/2+1:end-patch_w/2+1, patch_w/2+1:end-patch_w/2+1, :) = 1;
    end
    
end
