import os
import pandas as pd
import matplotlib.pyplot as plt


def plot_two_series(data, x_col, y1_col, y2_col, title, out_fname):
	plt.figure(figsize=(9, 5))
	x = data[x_col]
	plt.plot(x, data[y1_col], linestyle='-', label=y1_col)
	plt.plot(x, data[y2_col], linestyle='-', label=y2_col)
	plt.xlabel('vertices')
	plt.ylabel('nanoseconds')
	plt.title(title)
	plt.legend()
	plt.grid(alpha=0.4, linestyle='--')
	plt.tight_layout()
	plt.savefig(out_fname)
	plt.close()


def main():
	base = os.path.dirname(__file__)

	conn_path = os.path.join(base, 'connectivity_results.csv')
	mst_path = os.path.join(base, 'mst_results.csv')
	suitor_path = os.path.join(base, 'suitor_results.csv')

	# Connectivity
	conn = pd.read_csv(conn_path)
	print('connectivity.csv head:\n', conn.head())
	plot_two_series(conn, 'vertices', 'avg_add_edge_ns', 'avg_is_connected_ns',
					'Connectivity: add edge & is_connected',
					os.path.join(base, 'connectivity_plot.png'))

	# MST
	mst = pd.read_csv(mst_path)
	print('mst_results.csv head:\n', mst.head())
	plot_two_series(mst, 'vertices', 'avg_add_edge_ns', 'avg_is_in_mst_ns',
					'MST: add edge & is_in_mst',
					os.path.join(base, 'mst_plot.png'))

	# Suitor
	suitor = pd.read_csv(suitor_path)
	print('suitor_results.csv head:\n', suitor.head())
	plot_two_series(suitor, 'vertices', 'avg_add_edge_ns', 'avg_is_matched_ns',
					'Suitor: add edge & is_matched',
					os.path.join(base, 'suitor_plot.png'))


if __name__ == '__main__':
	main()
