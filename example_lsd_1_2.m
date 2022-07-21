%%
f = figure;
name = './images/test.jpg';
im = imread(name);
% subplot(1,2,1);
imshow(im, 'InitialMagnification','fit');hold on;
image = squeeze(mean(im,3));
% subplot(1,2,2);
% lines = lsd(name);
lines = mex_lsd(image');
for i = 1:size(lines, 2)
    plot(lines(1:2, i), lines(3:4, i), 'LineWidth', lines(5, i) / 2, 'Color', [1, 0, 0]);
end

%%
f = figure;
name = './images/test.jpg';
im = imread(name);
subplot(1,2,1);
imshow(im, 'InitialMagnification','fit');
image = squeeze(mean(im,3));

subplot(1,2,2);
pos = mex_lsd_2(image');
lsd_2_mat = zeros(size(im, 2), size(im, 1));
lsd_2_mat(pos) = 255;
lsd_2_mat = lsd_2_mat';
imshow(lsd_2_mat, 'InitialMagnification','fit');


